/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:05 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/22 15:56:51 by rnauke           ###   ########.fr       */
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

static size_t ft_stosize(const std::string& str)
{
	size_t num;
	std::stringstream ss(str);

	ss >> num;
	return num;
}

Server::Server() {}

// server will get initialized. That means a listening socket (serverSocket) will
// be created, set to non-blocking, set to be reused and binded to the local address.
Server::Server(struct pollfd* pfds, Clients* cd, Config& cfg) : _clientPollfds(pfds), _clientDetails(cd), _nfds(1)
{
	int reuse = 1;
  std::map<std::string, std::string> config = cfg.getConfig();
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
  this->_serverAdress.sin_port = htons(ft_stosh(config.find("listen")->second));

  std::cout << "server port: " << ft_stosh(config.find("listen")->second) << std::endl;
  this->_port = config.find("listen")->second;
  this->_servername = config.find("server_name")->second;
  this->_root = config.find("root")->second;
  this->_maxClientBody = ft_stosize(config.find("max_client_body")->second);
  this->_locations = cfg.getLocations();

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

// sends an answer to the client
void  Server::sendAnswer(size_t idx)
{
  static std::string tmp;
  const std::string requestType = this->_clientDetails[idx].getRequestType();
  for (std::vector<std::map<std::string, std::string> >::iterator i = this->_locations.begin(); i != _locations.end(); ++i)
	for (std::map<std::string, std::string>::iterator j = i.base()->begin(); j != i.base()->end(); ++j)
		std::cout << "location: " << j->first << "->" << j->second << std::endl;
	std::cout << std::endl;
	if (this->_clientDetails[idx].getConStatus() != CLOSE)
	{
		if (this->_clientDetails[idx].getMapValue("Version") != "HTTP/1.1")
			this->versionNotSupported(idx);
		else if (requestType == "GET")
			this->getMethod(idx, tmp);
		else if (requestType == "POST")
		{
			if (this->postMethod(idx) != 0)
			this->methodNotAllowed(idx);
		}
		else if (requestType == "DELETE")
			tmp = handle_file_erasing(this->_clientPollfds[idx].fd, this->_clientDetails[idx],
									this->_codes, this->_data);
		else
			this->notImplemented(idx);
	}

  if (this->_clientDetails[idx].getMapValue("Connection") != "keep-alive"
      || this->_clientDetails[idx].getConStatus() == CLOSE) {
    std::cout << "answer sent at idx: " << idx << " set back to POLLIN" << std::endl;
    this->removeFd(idx);
  }
  else if (this->_clientDetails[idx].getPendingResponse() == false) {
    this->_clientDetails[idx].cleanUp();
    this->_clientDetails[idx].cleanUpResponse();
    this->_clientPollfds[idx].events = POLLIN;
  }
}

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

