/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:05 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/17 15:03:40 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

std::string get_first_name(std::string body){
  size_t  start_pos = body.find_first_of("=") + 1;
  size_t  end_pos = body.find_first_of("&") - body.find_first_of("=") - 1;
  std::string ret_str = body.substr(start_pos, end_pos);

  return (ret_str);
}

std::string get_last_name(std::string body){
  size_t  start_pos = body.find_last_of("=") + 1;
  size_t  end_pos = body.size();
  std::string ret_str = body.substr(start_pos, end_pos);

  return (ret_str);
}

void  handle_Request_post(int fd, RequestParser req){
  char *argv[5];
  char cgi_filename[] = "first.cgi";
  std::string first_name = get_first_name(req.getBody());
  std::string last_name = get_last_name(req.getBody());
  std::string file_fd = std::to_string(fd);

  argv[0] = cgi_filename;
  argv[1] = const_cast<char*>(file_fd.c_str());
  argv[2] = const_cast<char*>(first_name.c_str());
  argv[3] = const_cast<char*>(last_name.c_str());
  argv[4] = NULL;
  execve("/Users/skunert/Documents/webserv/responseFiles/first.cgi", argv, NULL);
}

std::string  check_and_add_header(int status, std::string const& type, MIME_type data, Statuscodes codes){
  std::ostringstream header;
  header << "HTTP/1.1 " << status << " " << codes[status] << "\r\n";
  header << "Content-Type: "<< data[type] << "\r\n";
  header << "\r\n";
  return (header.str());
}

Server::Server(const ResponseFiles& responses) : _responses(responses)
{
  if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    throw(std::runtime_error(""));
  }

  this->_serverAdress.sin_family = AF_INET;
  this->_serverAdress.sin_addr.s_addr = INADDR_ANY;
  this->_serverAdress.sin_port = htons(PORT);

  // associates the server socket with the local address
  // and port specified in the "serverAddress" structure
  // if server is closed then socket goes into TIME_WAIT state and waits
  // for more incomming packages. This is a retrying mechanism that tries to
  // bind the socket for 20s. After that a timeout is thrown.
  int idx = 0;
  while (1)
  {
    if (bind(this->_serverSocket, reinterpret_cast<struct sockaddr*>(&_serverAdress), sizeof(_serverAdress)) == -1) {
      perror("bind");
      std::cout << "Retrying..." << std::endl;
      sleep(1);
      if (idx++ == 20) {
        close(this->_serverSocket);
        throw(std::runtime_error("Error timeouted trying to bind socket"));
      }
      continue;
    }
    break;
  }

  // listens for incomming connection requests
  if (listen(this->_serverSocket, SOMAXCONN) == -1) {
    close(this->_serverSocket);
    perror("listen");
    throw(std::runtime_error("Error listening for connections"));
  }
}

Server::~Server()
{
  close(this->_serverSocket);
}

void  Server::handleRequest(std::map<std::string, std::string>& files, std::string type, MIME_type data, Statuscodes codes)
{
  if (this->_requests.getRequestType() == "GET")
  {
    if (this->_requests.getUri() == "/")
      send(this->_clientSocket, (check_and_add_header(200, type, data, codes) + files[this->_requests.getUri()]).c_str(),
         (check_and_add_header(200, type, data, codes) + files[this->_requests.getUri()]).size(), 0);
    else if (this->_requests.getUri() == "/image.webp")
      send(this->_clientSocket, (check_and_add_header(200, type, data, codes) + files[this->_requests.getUri()]).c_str(),
         (check_and_add_header(200, type, data, codes) + files[this->_requests.getUri()]).size(), 0);
    else if (this->_requests.getUri() == "/background.webp")
      send(this->_clientSocket, (check_and_add_header(200, type, data, codes) + files[this->_requests.getUri()]).c_str(),
         (check_and_add_header(200, type, data, codes) + files[this->_requests.getUri()]).size(), 0);
    else if (this->_requests.getUri() == "/shutdown") {
      close(this->_clientSocket);
      close(this->_serverSocket);
      exit(EXIT_SUCCESS);
    }
    else{
      send(this->_clientSocket, (check_and_add_header(404, type, data, codes) + files["error"]).c_str(),
         (check_and_add_header(404, type, data, codes) + files["error"]).size(), 0);
    }
  }
  if (this->_requests.getRequestType() == "POST"){
    int pid = fork();
    if (pid == 0){
        handle_Request_post(this->_clientSocket, this->_requests);
    }
    waitpid(0, NULL, 0);
  }
  std::cout << this->_clientSocket << std::endl;
}

void  Server::serverLoop(MIME_type data, Statuscodes codes)
{
  std::map<std::string, std::string> files(this->_responses.getResponseFiles());
  while (true)
  {
    std::cout << "Waiting for client..." << std::endl;

    if ((this->_clientSocket = accept(this->_serverSocket, NULL, NULL)) == -1) {
      const std::string errorResponse = "503 Service Unavailable\n";
      send(this->_clientSocket, errorResponse.c_str(), errorResponse.size(), 0);
      close(this->_clientSocket);
      continue;
    }
    std::cout << "Proccessing request..." << std::endl;

    char buffer[1024];
    ssize_t bytesRead;
    bytesRead = recv(this->_clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead < 0) {
      close(this->_clientSocket);
      throw(std::runtime_error("Error in recieving client data"));
    }
    if (bytesRead == 0) {
      std::cout << "client has closed the connection" << std::endl;
      close(this->_clientSocket);
      continue;
    }
    buffer[bytesRead] = '\0';
    std::cout << buffer << std::endl;
    this->_requests.parseRequestBuffer(buffer);
    this->handleRequest(files, this->_requests.getRequestType(), data, codes);
    this->_requests.cleanUp();
  }
  close(this->_clientSocket);
}
