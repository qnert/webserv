/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:16 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/25 12:41:22 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ResponseFiles.hpp"
#include "RequestParser.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <poll.h>
#include <vector>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <cstdlib>

#define PORT 8080

class Server
{
private:
  int                        _reuse;
  struct pollfd              _clientPollfds[200];
  nfds_t                     _nfds;
  size_t                     _currSize;
  int                        _serverSocket;
  sockaddr_in                _serverAdress;
  const ResponseFiles        _responses;
  RequestParser              _requests;

  void  sendAnswer(std::map<std::string, std::string>& files, std::string type, MIME_type& data, Statuscodes& codes, size_t idx);
  void  handleRequest(std::map<std::string, std::string>& files, MIME_type& data, Statuscodes& codes, int i);
  void  checkRevents(int i);
  void  acceptConnections(void);
  void  cleanUpClientFds();
public:
  Server(const ResponseFiles& responses);
  ~Server();

  void  serverLoop(MIME_type& data, Statuscodes& codes);
};
