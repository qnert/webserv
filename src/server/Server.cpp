/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:05 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/14 16:28:14 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

Server::Server(const ResponseFiles& responses) : _responses(responses)
{
  if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    throw(std::runtime_error("Error getting socket"));

  this->_serverAdress.sin_family = AF_INET;
  this->_serverAdress.sin_addr.s_addr = INADDR_ANY;
  this->_serverAdress.sin_port = htons(PORT);

  // associates the server socket with the local address
  // and port specified in the "serverAddress" structure
  if (bind(this->_serverSocket, reinterpret_cast<struct sockaddr*>(&_serverAdress), sizeof(_serverAdress)) == -1) {
    close(this->_serverSocket);
    throw(std::runtime_error("Error binding socket"));
  }

  // listens for incomming connection requests
  if (listen(this->_serverSocket, SOMAXCONN) == -1) {
    close(this->_serverSocket);
    throw(std::runtime_error("Error listening for connections"));
  }
}

Server::~Server()
{
  close(this->_serverSocket);
}

void  Server::handleRequest(std::map<std::string, std::string>& files)
{
  static int i;
  if (i == 0){
    i++;
    send(this->_clientSocket, files["index"].c_str(), files["index"].size(), 0);
  }
  else{
    send(this->_clientSocket, ("HTTP/1.1 200 OK\r\nContent-Type: image/gif\r\n\r\n" + files["giphy"]).c_str(), 50 + files["giphy"].size(), 0);
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
    this->handleRequest(files);
  }
  close(this->_clientSocket);
}
