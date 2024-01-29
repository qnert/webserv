/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:05 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/29 16:43:47 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

std::string  storeFileIntoString(RequestParser req, std::string path)
{
  if (req.getUri() == "/")
    path = req.getCurrdir() + "responseFiles/index.html";
  else
    path = req.getCurrdir() + path;
  std::ifstream file(path, std::ios::binary);
  if (!file.is_open())
    return ("");

  std::string fileContent;
  std::ostringstream buffer;
  buffer << file.rdbuf();
  fileContent = buffer.str();
  file.close();
  return (fileContent);
}

std::string get_first_name(std::string body){
  size_t  start_pos = body.find_first_of("=") + 1;
  size_t  end_pos = body.find_first_of("&") - body.find_first_of("=") - 1;
  std::string ret_str = body.substr(start_pos, end_pos);

  return (ret_str);
}

std::string get_last_name(std::string body){
  size_t  start_pos = body.find_last_of("=") + 1;
  size_t  end_pos = body.find_last_of("&") - body.find_last_of("=") - 1;
  std::string ret_str = body.substr(start_pos, end_pos);

  return (ret_str);
}

std::string get_filecontent(std::string body){
  size_t start = body.find("\n\n") + 2;
  size_t end = body.find("\n------");
  std::string filecontent = body.substr(start, end - start);

  return (filecontent);
}

std::string get_filename(std::string body){
  std::string filename = body.substr(body.find("filename=") + 10);
  filename = filename.substr(0, filename.find('\n') - 2);
  return (filename);
}

void  handle_name_input(int fd, RequestParser req){
  char *argv[5];
  std::string cgi_filename = req.getUri().substr(req.getUri().find_last_of("/") + 1, req.getUri().size());
  std::string file_fd = std::to_string(fd);
  std::string first_name = get_first_name(req.getBody());
  std::string last_name = get_last_name(req.getBody());

  argv[0] = const_cast<char*>(cgi_filename.c_str());
  argv[1] = const_cast<char*>(file_fd.c_str());
  argv[2] = const_cast<char*>(first_name.c_str());
  argv[3] = const_cast<char*>(last_name.c_str());
  argv[4] = NULL;
  execve((req.getCurrdir() + req.getUri()).c_str(), argv, NULL);
}

void  handle_file_upload(int fd, RequestParser req){
  char *argv[5];
  std::string cgi_filename = "./cpp_uploadfile.cgi";
  std::string file_fd = std::to_string(fd);
  std::string filename = get_filename(req.getBody());
  std::string filecontent = get_filecontent(req. getBody());
  argv[0] = const_cast<char*>(cgi_filename.c_str());
  argv[1] = const_cast<char*>(file_fd.c_str());
  argv[2] = const_cast<char*>(filename.c_str());
  argv[3] = const_cast<char*>(filecontent.c_str());
  argv[4] = NULL;
  execve("/Users/skunert/Documents/webserv/responseFiles/cpp_uploadfile.cgi", argv, NULL);
}

void  handle_Request_post(int fd, RequestParser req){
  if (req.getUri() == "/responseFiles/first.cgi")
    handle_name_input(fd, req);
  else if (req.getUri() == "upload")
    handle_file_upload(fd, req);
}

std::string  check_and_add_header(int status, std::string const& type, MIME_type data, Statuscodes codes){
  std::ostringstream header;
  header << "HTTP/1.1 " << status << " " << codes[status] << "\r\n";
  header << "Content-Type: "<< data[type] << "\r\n";
  header << "\r\n";
  return (header.str());
}

// server will get initialized. That means a listening socket (serverSocket) will
// be created, set to non-blocking, set to be reused and binded to the local address.
Server::Server() : _reuse(1), _nfds(1), _currSize(0)
{
  if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    throw(std::runtime_error(""));
  }

  setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &this->_reuse, sizeof(this->_reuse));
  this->_serverAdress.sin_family = AF_INET;
  this->_serverAdress.sin_addr.s_addr = INADDR_ANY;
  this->_serverAdress.sin_port = htons(PORT);

  // associates the server socket with the local address
  // and port specified in the "serverAddress" structure
  if (bind(this->_serverSocket, reinterpret_cast<struct sockaddr*>(&_serverAdress), sizeof(_serverAdress)) == -1) {
    perror("bind");
    close(this->_serverSocket);
    throw(std::runtime_error("Error timeouted trying to bind socket"));
  }

  // listens for incomming connection requests
  if (listen(this->_serverSocket, SOMAXCONN) == -1) {
    close(this->_serverSocket);
    perror("listen");
    throw(std::runtime_error("Error listening for connections"));
  }

  if (fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
    perror("fcntl server");

  struct pollfd serverPollfd;
  serverPollfd.fd = this->_serverSocket;
  serverPollfd.events = POLLIN;
  serverPollfd.revents = 0;
  this->_clientPollfds[0] = serverPollfd;
}

Server::~Server() {}

// sends an answer to the client
void  Server::sendAnswer(MIME_type& data, Statuscodes& codes, size_t idx)
{
  if (this->_requests.getRequestType() == "GET")
  {
    std::string msg = storeFileIntoString(this->_requests, this->_requests.getUri());
    if (msg != "")
    {
      send(this->_clientPollfds[idx].fd, (check_and_add_header(200, this->_requests.getRequestType(), data, codes) + msg).c_str(),
         (check_and_add_header(200, this->_requests.getRequestType(), data, codes) + msg).size(), 0);
    }
    else if (this->_requests.getUri() == "/shutdown")
    {
      this->cleanUpClientFds();
      exit(EXIT_SUCCESS);
    }
    else
    {
      msg = storeFileIntoString(this->_requests, "responseFiles/error.html");
      send(this->_clientPollfds[idx].fd, (check_and_add_header(404, ".html", data, codes) + msg).c_str(),
         (check_and_add_header(404, ".html", data, codes) + msg).size(), 0);
      return ;
    }
  }
  if (this->_requests.getRequestType() == "POST" && this->_requests.getUri() == "/responseFiles/first.cgi")
  {
    int pid = fork();
    if (pid == 0)
        handle_Request_post(this->_clientPollfds[idx].fd, this->_requests);
    waitpid(0, NULL, 0);
  }
  else if (this->_requests.getUri() == "upload")
  {
    int pid = fork();
    if (pid == 0)
        handle_Request_post(this->_clientPollfds[idx].fd, this->_requests);
    waitpid(0, NULL, 0);
  }
}

// checks if readable data is available at the client socket
void  Server::checkRevents(int i)
{
  if (this->_clientPollfds[i].revents != POLLIN)
  {
    if (this->_clientPollfds[i].revents & POLLHUP)
      this->removeAndCompressFds(i);
    else if (this->_clientPollfds[i].revents & POLLERR)
    {
      perror("POLLERR");
      this->cleanUpClientFds();
      throw(std::runtime_error(""));
    }
  }
}

// accept every client in that wants to connect
void  Server::acceptConnections(void)
{
  int newClientSocket;
  do
  {
    if (this->_nfds == MAX_CLIENTS) {
      std::cout << "Maximum amount of clients reached" << std::endl;
      break ;
    }
    if ((newClientSocket = accept(this->_serverSocket, NULL, NULL)) == -1)
      break;

    std::cout << "New client connected..." << std::endl;

    if (fcntl(newClientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
      perror("fcntl client");

    struct pollfd clientFd;
    clientFd.fd = newClientSocket;
    clientFd.events = POLLIN;
    clientFd.revents = 0;
    this->_clientPollfds[this->_nfds] = clientFd;
    this->_timestamp[this->_nfds] = std::time(NULL);
    this->_nfds++;
  } while (newClientSocket != -1);
}

// recieves, parses and handles client requests
void  Server::handleRequest(MIME_type& data, Statuscodes& codes, int i)
{
  while (true)
  {
    char buffer[10000];
    ssize_t bytesRead = recv(this->_clientPollfds[i].fd, buffer, sizeof(buffer), 0);

    if (bytesRead < 0) {
      break;
    }
    if (bytesRead == 0) {
      std::cout << "Client has closed the connection" << std::endl;
      close(this->_clientPollfds[i].fd);
      break;
    }
    buffer[bytesRead] = '\0';
    this->_requests.parseRequestBuffer(buffer);
    this->sendAnswer(data, codes, i);
    this->_requests.cleanUp();
  }
}

// main server loop
void  Server::serverLoop(MIME_type& data, Statuscodes& codes)
{
  while (true)
  {
    std::cout << "Waiting for poll()..." << std::endl;
      if (poll(this->_clientPollfds, this->_nfds, 10000) < 0) {
        perror("poll");
        close(this->_serverSocket);
        throw(std::runtime_error(""));
      }

    this->_currSize = this->_nfds;
    for (size_t i = 0; i < this->_currSize; i++)
    {
      this->checkClientTimeout(i);

      if (this->_clientPollfds[i].revents == 0)
        continue;

      this->checkRevents(i);

      if (this->_clientPollfds[i].fd == this->_serverSocket)
        this->acceptConnections();
      else
        this->handleRequest(data, codes, i);
    } // * END OF CLIENT LOOP *
  } // * END OF SERVER *
  this->cleanUpClientFds();
}
