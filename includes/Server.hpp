/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:16 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/23 13:25:47 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "RequestParser.hpp"
#include "MIME_type.hpp"
#include "Statuscodes.hpp"
#include <iostream>
#include <unistd.h>
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
#include <ctime>

#define PORT 8080
#define MAX_CLIENTS 200
#define CLIENT_TIMEOUT 5

class Server
{
private:
  time_t                     _timestamp[MAX_CLIENTS];
  int                        _reuse;
  struct pollfd              _clientPollfds[MAX_CLIENTS];
  nfds_t                     _nfds;
  size_t                     _currSize;
  int                        _serverSocket;
  sockaddr_in                _serverAdress;
  const ResponseFiles        _responses;
  RequestParser              _requests;

  void  sendAnswer(RequestParser& req, MIME_type& data, Statuscodes& codes, size_t idx);
  void  handleRequest(RequestParser& req, MIME_type& data, Statuscodes& codes, int i);
  void  checkRevents(int i);
  void  acceptConnections(void);
  void  cleanUpClientFds();
  void  removeAndCompressFds(int i);
  void  checkClientTimeout(int i);
public:
  Server();
  ~Server();

  void  serverLoop(MIME_type& data, Statuscodes& codes);
};
