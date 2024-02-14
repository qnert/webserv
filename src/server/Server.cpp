/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:05 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/14 15:07:11 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

// server will get initialized. That means a listening socket (serverSocket) will
// be created, set to non-blocking, set to be reused and binded to the local address.
Server::Server(MIME_type& data, Statuscodes& codes) : _data(data), _codes(codes), _reuse(1), _nfds(1), _currSize(0)
{
  clientsInit();
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
    std::cout << "Connection closed on idx: " << idx << std::endl;
  }
  else if (this->_clientDetails[idx].getPendingResponse() == false) {
    this->_clientDetails[idx].cleanUp();
    this->_clientDetails[idx].cleanUpResponse();
    this->_clientPollfds[idx].events = POLLIN;
  }
}

// checks if readable data is available at the client socket
void  Server::checkRevents(int i)
{
  int error = 0;

  if (this->_clientPollfds[i].revents & POLLHUP)
    this->removeFd(i);
  else if (this->_clientPollfds[i].revents & POLLERR)
    error = 1;
  else if (this->_clientPollfds[i].revents & POLLNVAL)
    error = 1;
  if (error == 1)
  {
    perror("poll_revents");
    this->cleanUpClientFds();
    throw(std::runtime_error(""));
  }
}

// accept every client in that wants to connect
void  Server::acceptConnections(void)
{
  int newClientSocket;

  if (this->_nfds == MAX_CLIENTS) {
    std::cout << "Maximum amount of clients reached" << std::endl;
    return ;
  }
  if ((newClientSocket = accept(this->_serverSocket, NULL, NULL)) == -1)
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

// recieves, parses and handles client requests
void  Server::handleRequest(int i)
{
  while (true)
  {
    char buffer[10000];
    ssize_t bytesRead = recv(this->_clientPollfds[i].fd, buffer, sizeof(buffer) - 1, O_NONBLOCK);

    if (bytesRead < 0) {
      break;
    }
    if (bytesRead == 0 && this->_clientDetails[i].getPendingReceive() == false) {
      std::cout << "Client has closed the connection" << std::endl;
      this->removeFd(i);
      break;
    }
    buffer[bytesRead] = '\0';
    this->_clientDetails[i].parseRequestBuffer(buffer, bytesRead);
    if (this->_clientDetails[i].getPendingReceive() == false)
      this->_clientPollfds[i].events = POLLOUT;
  }
}

// main server loop
void  Server::serverLoop()
{
  while (true)
  {
    if (poll(this->_clientPollfds, MAX_CLIENTS, 10000) < 0) {
      perror("poll");
      close(this->_serverSocket);
      throw(std::runtime_error(""));
    }

    for (size_t i = 0; i < MAX_CLIENTS; i++)
    {
      if (this->_clientPollfds[i].revents == 0)
        continue;

      this->checkRevents(i);

      if (this->_clientPollfds[i].revents == POLLIN)
      {
        if (this->_clientPollfds[i].fd == this->_serverSocket)
          this->acceptConnections();
        else
          this->handleRequest(i);
      }
      else if (this->_clientPollfds[i].revents == POLLOUT)
      {
        this->sendAnswer(i);
      }
    } // * END OF CLIENT LOOP *
  } // * END OF SERVER *
  this->cleanUpClientFds();
}
