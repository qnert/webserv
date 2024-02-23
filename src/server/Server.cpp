/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:05 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/23 16:33:02 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

Server::Server() {}

// server will get initialized. That means a listening socket (serverSocket) will
// be created, set to non-blocking, set to be reused and binded to the local address.
Server::Server(struct pollfd* pfds, Clients* cd, Config& cfg) : _clientPollfds(pfds), _clientDetails(cd), _nfds(1)
{
	int reuse = 1;
  initConfVars(cfg);
  if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    throw(std::runtime_error(""));
  }

  if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
    perror("setsockopt");
    close(this->_serverSocket);
    throw(std::runtime_error(""));
  }

  this->_serverAdress.sin_family = AF_INET;
  this->_serverAdress.sin_addr.s_addr = INADDR_ANY;
  this->_serverAdress.sin_port = htons(ft_stosh(this->_port));

  // associates the server socket with the local address
  // and port specified in the "serverAddress" structure
  if (bind(this->_serverSocket, reinterpret_cast<struct sockaddr*>(&_serverAdress), sizeof(_serverAdress)) == -1)
  {
    perror("bind");
    close(this->_serverSocket);
    throw(std::runtime_error("Error timeouted trying to bind socket"));
  }

  // listens for incomming connection requests
  if (listen(this->_serverSocket, 200) == -1) {
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
  int index = getFreeSocket();
  this->_clientPollfds[index] = serverPollfd;
  this->_clientDetails[index].setFdStatus(USED);
}

Server::~Server() {}

// accept every client in that wants to connect
void  Server::acceptConnections()
{
  int newClientSocket;

  if (this->_nfds == MAX_CLIENTS) {
    std::cout << "Maximum amount of clients reached" << std::endl;
    return ;
  }

  socklen_t serverAdressLen = sizeof(this->_serverAdress);
  newClientSocket = accept(this->_serverSocket,
                                reinterpret_cast<struct sockaddr*>(&this->_serverAdress),
                                &serverAdressLen);
  if (newClientSocket == -1)
  {
    perror("accept");
    return ;
  }

  if (fcntl(newClientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
    perror("fcntl client");

  int index = this->getFreeSocket();
  this->_clientPollfds[index].fd = newClientSocket;
  this->_clientDetails[index].setFdStatus(USED);
  this->_nfds++;
  std::cout << "New client connected at index: " << index << std::endl;
}

int Server::getFd()
{
	return _serverSocket;
}

std::string Server::getServername()
{
	return _servername;
}

std::string Server::getPort()
{
	return _port;
}
std::string Server::getRoot()
{
	return _root;
}

