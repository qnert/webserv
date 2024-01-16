/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:05 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/16 10:08:45 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

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

void  Server::handleRequest(std::map<std::string, std::string>& files)
{
  if (this->_requests.getRequestType() == "GET")
  {
    if (this->_requests.getUri() == "/")
      send(this->_clientSocket, files["index"].c_str(), files["index"].size(), 0);
    else if (this->_requests.getUri() == "/image.webp")
      send(this->_clientSocket, files["image"].c_str(), files["image"].size(), 0);
    else if (this->_requests.getUri() == "/background.webp")
      send(this->_clientSocket, files["backg"].c_str(), files["backg"].size(), 0);
    else if (this->_requests.getUri() == "/shutdown") {
      close(this->_clientSocket);
      close(this->_serverSocket);
      exit(EXIT_SUCCESS);
    }
    else{
      send(this->_clientSocket, files["error"].c_str(), files["error"].size(), 0);
    }
  }
}

void  Server::serverLoop()
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
    this->handleRequest(files);
    this->_requests.cleanUp();
  }
  close(this->_clientSocket);
}
