/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:16 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/19 03:41:34 by rnauke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Clients.hpp"
#include "Config.hpp"
#include "RequestUtils.hpp"
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <vector>
#include <cstdio>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENTS 200

class Server
{
private:
  MIME_type           _data;
  Statuscodes         _codes;
  struct pollfd*       _clientPollfds;
  Clients*             _clientDetails;
  int                 _reuse;
  nfds_t              _nfds;
  size_t              _currSize;
  int                 _serverSocket;
  sockaddr_in         _serverAdress;
  
  std::string _servername;
  std::string _port;

  void                getMethod(size_t idx, std::string& tmp);
  int                 postMethod(size_t idx);
  void                notImplemented(size_t idx);
  void                methodNotAllowed(size_t idx);

  int                 getFreeSocket();
  void                clientsInit();
  void                handleRequest(int i);
  void                checkRevents(int i);
//   void                acceptConnections(void);
  void                cleanUpClientFds();
  void                removeFd(int i);

	void createServerSockets(std::vector<std::map<std::string, std::string> > configs);
	bool isServerSocket(int fd);
public:
  Server(MIME_type& data, Statuscodes& codes, struct pollfd* pfds, Clients* cd, std::map<std::string, std::string> cfg);
  ~Server();

  void                serverLoop(void);
  static std::string  ft_itos(size_t num);
  Statuscodes&        getStatuscodes(void);
  MIME_type&          getMimeType(void);

	int getFd();
	void                acceptConnections();
  	void                sendAnswer(size_t idx);
	std::string getServername();
	std::string getPort();
};
