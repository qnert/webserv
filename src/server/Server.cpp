/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:05 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/22 16:06:37 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

std::string  check_and_add_header(int status, std::string const& type, MIME_type data, Statuscodes codes){
  std::ostringstream header;
  header << "HTTP/1.1 " << status << " " << codes[status] << "\r\n";
  header << "Content-Type: "<< data[type] << "\r\n";
  header << "\r\n";
  return (header.str());
}

Server::Server(const ResponseFiles& responses) : _reuse(1), _nfds(1), _currSize(0), _responses(responses)
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

  fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);

  struct pollfd serverPollfd;
  serverPollfd.fd = this->_serverSocket;
  serverPollfd.events = POLLIN;
  this->_clientPollfds[0] = serverPollfd;
}

Server::~Server() {}

void  Server::handleRequest(std::map<std::string, std::string>& files, std::string type, MIME_type data, Statuscodes codes, size_t idx)
{
  if (this->_requests.getRequestType() == "GET")
  {
    if (this->_requests.getUri() == "/")
      send(this->_clientPollfds[idx].fd, (check_and_add_header(200, type, data, codes) + files[this->_requests.getUri()]).c_str(),
         (check_and_add_header(200, type, data, codes) + files[this->_requests.getUri()]).size(), 0);
    else if (this->_requests.getUri() == "/image.webp")
      send(this->_clientPollfds[idx].fd, (check_and_add_header(200, type, data, codes) + files[this->_requests.getUri()]).c_str(),
         (check_and_add_header(200, type, data, codes) + files[this->_requests.getUri()]).size(), 0);
    else if (this->_requests.getUri() == "/background.webp")
      send(this->_clientPollfds[idx].fd, (check_and_add_header(200, type, data, codes) + files[this->_requests.getUri()]).c_str(),
         (check_and_add_header(200, type, data, codes) + files[this->_requests.getUri()]).size(), 0);
    else if (this->_requests.getUri() == "/shutdown") {
      this->cleanUpClientFds();
      exit(EXIT_SUCCESS);
    }
    else{
      send(this->_clientPollfds[idx].fd, (check_and_add_header(404, type, data, codes) + files["error"]).c_str(),
         (check_and_add_header(404, type, data, codes) + files["error"]).size(), 0);
    }
  }
}

void  Server::serverLoop(MIME_type data, Statuscodes codes)
{
  std::map<std::string, std::string> files(this->_responses.getResponseFiles());
  int timeout = (60 * 1000);
  int functionStatus = 0;
  while (true)
  {
    std::cout << "Waiting for poll()..." << std::endl;
    functionStatus = poll(this->_clientPollfds, this->_nfds, timeout);

    if (functionStatus < 0) {
      perror("poll");
      close(this->_serverSocket);
      throw(std::runtime_error(""));
    }
    if (functionStatus == 0) {
      std::cout << "poll() timed out. Closing server..." << std::endl;
      close(this->_serverSocket);
      return ;
    }

    this->_currSize = this->_nfds;
    for (size_t i = 0; i < this->_currSize; i++)
    {
      if (this->_clientPollfds[i].revents == 0)
        continue;

      if (this->_clientPollfds[i].revents != POLLIN)
      {
        if (this->_clientPollfds[i].revents & POLLHUP)
        {
          close(this->_clientPollfds[i].fd);
          for (size_t j = i; j < this->_nfds - 1; j++)
            this->_clientPollfds[j] = this->_clientPollfds[j + 1];
          std::cout << "before closing: " << this->_nfds << std::endl;
          this->_nfds--;
          std::cout << "after closing: " << this->_nfds << std::endl;
        }
        else if (this->_clientPollfds[i].revents & POLLERR)
        {
          perror("POLLERR");
          this->cleanUpClientFds();
          throw(std::runtime_error(""));
        }
      }

      if (this->_clientPollfds[i].fd == this->_serverSocket)
      {
        int newClientSocket;
        do
        {
          if ((newClientSocket = accept(this->_serverSocket, NULL, NULL)) == -1)
            break;

          std::cout << "New client connected..." << std::endl;

          if (fcntl(newClientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
            perror("fnctl client");

          struct pollfd clientFd;
          clientFd.fd = newClientSocket;
          clientFd.events = POLLIN;
          clientFd.revents = 0;
          this->_clientPollfds[this->_nfds] = clientFd;
          std::cout << this->_nfds << std::endl;
          this->_nfds++;
        } while (newClientSocket != -1);
      }
      else
      {
        while (true)
        {
          char buffer[1024];
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
          std::cout << buffer << std::endl;
          this->_requests.parseRequestBuffer(buffer);
          this->handleRequest(files, this->_requests.getRequestType(), data, codes, i);
          this->_requests.cleanUp();
        }
      }
    } // * END OF CLIENT LOOP *
  } // * END OF WHILE 1 *
  this->cleanUpClientFds();
}
