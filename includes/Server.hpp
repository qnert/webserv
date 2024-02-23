/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:16 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/23 18:54:22 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Clients.hpp"
#include "Config.hpp"
#include "RequestUtils.hpp"
#include "CGI.hpp"
#include "Utils.hpp"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <poll.h>
#include <cstdio>
#include <dirent.h>
#include <unistd.h>

#define MAX_CLIENTS 200

typedef std::map<std::string, std::string> t_strmap;

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
  std::string         _index;
  size_t              _maxClientBody;

  std::vector<t_strmap> _locations;
  t_strmap              _currLocation;
  t_strmap              _error_pages; // key = Error code ("404"->"./path/to/file")

  // ResponseMethods
  void                getMethod(size_t idx, std::string& tmp);
  int                 postMethod(size_t idx);
  void                handleGetDefault(std::string& msg, size_t idx);

  // ProcessResponse
  void                chooseMethod(size_t idx);

  // ErrorResponses
  void                notImplemented(size_t idx);
  void                methodNotAllowed(size_t idx);
  void                NotFound(size_t idx);
  void                versionNotSupported(size_t idx);
  void                payloadTooLarge(size_t idx);

  int                 getFreeSocket();
  void                cleanUpClientFds();

  // Location
  void                initConfVars(Config& cfg);
  bool                checkLocationPrelims(std::string method);
public:
  Server();
  Server(struct pollfd* pfds, Clients* cd, Config& cfg);
  ~Server();

  void                removeFd(int i);
  Statuscodes&        getStatuscodes(void);
  MIME_type&          getMimeType(void);

  // ProcessResponse
  void                sendAnswer(size_t idx);

	int                 getFd();
	void                acceptConnections();
	std::string         getServername();
	std::string         getPort();
	std::string         getRoot();

  // Location
  void                getCurrLocation(size_t index);
};
