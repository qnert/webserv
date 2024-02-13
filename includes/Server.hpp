/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:16 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/13 14:46:18 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "RequestParser.hpp"
#include "RequestUtils.hpp"
#include "CGI.hpp"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <poll.h>
#include <vector>
#include <cstdio>
#include <unistd.h>
#include <string>

#define PORT 8080
#define MAX_CLIENTS 200

class Server
{
private:
  RequestParser       _requests;
  MIME_type           _data;
  Statuscodes         _codes;
  int                 _reuse;
  struct pollfd       _clientPollfds[MAX_CLIENTS];
  nfds_t              _nfds;
  size_t              _currSize;
  int                 _serverSocket;
  sockaddr_in         _serverAdress;
  bool                _pendingResponse;

  void                sendAnswer(size_t idx);
  void                getMethod(size_t idx, std::string& tmp);
  int                 postMethod(size_t idx);
  void                notImplemented(size_t idx);
  void                methodNotAllowed(size_t idx);

  void                handleRequest(int i);
  void                checkRevents(int i);
  void                acceptConnections(void);
  void                cleanUpClientFds();
  void                removeAndCompressFds(int i);
public:
  Server(MIME_type& data, Statuscodes& codes);
  ~Server();

  void                serverLoop(void);
  static std::string  ft_itos(size_t num);
  Statuscodes&        getStatuscodes(void);
  MIME_type&          getMimeType(void);
};
