/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:05 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/19 20:13:34 by rnauke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

static short ft_stosh(const std::string& str)
{
	short num;
	std::stringstream ss(str);

	ss >> num;
	return num;
}

// server will get initialized. That means a listening socket (serverSocket) will
// be created, set to non-blocking, set to be reused and binded to the local address.
Server::Server(MIME_type& data, Statuscodes& codes, struct pollfd* pfds, Clients* cd, std::map<std::string, std::string> cfg) : _data(data), _codes(codes), _clientPollfds(pfds), _clientDetails(cd), _nfds(1)
{
	int reuse = 1;
//   clientsInit();
  if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    throw(std::runtime_error(""));
  }

  setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
  this->_serverAdress.sin_family = AF_INET;
  this->_serverAdress.sin_addr.s_addr = INADDR_ANY;
  this->_serverAdress.sin_port = htons(ft_stosh(cfg.find("listen")->second));

  std::cout << "server port: " << ft_stosh(cfg.find("listen")->second) << std::endl;
  this->_port = cfg.find("listen")->second;
  this->_servername = cfg.find("server_name")->second;
//   this->_root = cfg.find("root")->second;
  
  // associates the server socket with the local address
  // and port specified in the "serverAddress" structure
  if (bind(this->_serverSocket, reinterpret_cast<struct sockaddr*>(&_serverAdress), sizeof(_serverAdress)) == -1)
  {
    perror("bind");
    close(this->_serverSocket);
    throw(std::runtime_error("Error timeouted trying to bind socket"));
  }

  // listens for incomming connection requests
  if (listen(this->_serverSocket, SOMAXCONN) == -1)
  {
    close(this->_serverSocket);
    perror("listen");
    throw(std::runtime_error("Error listening for connections"));
  }

  if (fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
    perror("fcntl server");

  struct pollfd serverfd;
  serverfd.fd = this->_serverSocket;
  serverfd.events = POLLIN;
  serverfd.revents = 0;
  std::cout << "fd: " << _serverSocket << std::endl;
  std::cout << "index in array: (call)" << getFreeSocket() << std::endl;
  std::cout << "state of array: " << _clientPollfds[getFreeSocket()].fd << std::endl;
	this->_clientPollfds[getFreeSocket()] = serverfd;
}

Server::~Server() {}

// sends an answer to the client
void  Server::sendAnswer(size_t idx)
{
  static std::string tmp;
  const std::string requestType = this->_clientDetails[idx].getRequestType();

  if (requestType == "GET")
    this->getMethod(idx, tmp);
  else if (requestType == "POST" || this->_clientDetails[idx].getUri() == "upload")
  {
    if (this->postMethod(idx) != 0)
      this->methodNotAllowed(idx);
  }
  else if (requestType == "DELETE")
    tmp = handle_file_erasing(this->_clientPollfds[idx].fd, this->_clientDetails[idx], this->_codes);
  else
    this->notImplemented(idx);

  if (this->_clientDetails[idx].getMapValue("Connection") != "keep-alive") {
    this->removeFd(idx);
    std::cout << "Connection closed on a fd" << std::endl;
  }
  else
    this->_clientPollfds[idx].events = POLLIN;
}

// accept every client in that wants to connect
void  Server::acceptConnections()
{
  int newClientSocket;

  if (this->_nfds == MAX_CLIENTS) {
    std::cout << "Maximum amount of clients reached" << std::endl;
    return ;
  }
  if ((newClientSocket = accept(_serverSocket, NULL, NULL)) == -1)
    return ;

  std::cout << "New client connected..." << std::endl;

  if (fcntl(newClientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
    perror("fcntl client");

  struct pollfd clientFd;
  clientFd.fd = newClientSocket;
  clientFd.events = POLLIN;
  clientFd.revents = 0;
  int index = this->getFreeSocket();
  this->_clientPollfds[index] = clientFd;
  this->_nfds++;
  std::cout << "number of clients connected now: " << this->_nfds << std::endl;
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