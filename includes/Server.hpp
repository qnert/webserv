/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:16 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/20 16:01:56 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Clients.hpp"
#include "Config.hpp"
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
#include <cstdio>
#include <unistd.h>

#define MAX_CLIENTS 200

class Server
{
private:
  MIME_type           _data;
  Statuscodes         _codes;
  struct pollfd*      _clientPollfds;
  Clients*            _clientDetails;
  int                 reuse;
  nfds_t              _nfds;
  size_t              _currSize;
  int                 _serverSocket;
  sockaddr_in         _serverAdress;

  std::string         _servername;
  std::string         _port;
  std::string         _root;
  bool                _defaultserver;

  void                getMethod(size_t idx, std::string& tmp);
  int                 postMethod(size_t idx);
  void                notImplemented(size_t idx);
  void                methodNotAllowed(size_t idx);
  void                handleGetDefault(std::string& msg, size_t idx);
  void                NotFound(size_t idx);
  void                versionNotSupported(size_t idx);

  int                 getFreeSocket();
//   void                handleRequest(int i);
//   void                checkRevents(int i);
//   void                acceptConnections(void);
  void                cleanUpClientFds();

	// void createServerSockets(std::vector<std::map<std::string, std::string> > configs);
	// bool isServerSocket(int fd);
public:
  Server();
  Server(MIME_type& data, Statuscodes& codes, struct pollfd* pfds, Clients* cd, std::map<std::string, std::string> cfg);
  ~Server();

//   void                serverLoop(void);
  static void         removeFd(int i);
  static std::string  ft_itos(size_t num);
  Statuscodes&        getStatuscodes(void);
  MIME_type&          getMimeType(void);

	int                 getFd();
	void                acceptConnections();
  void                sendAnswer(size_t idx);
	std::string         getServername();
	std::string         getPort();
	std::string         getRoot();
  bool                isDefaultServer();
};
