/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:16 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/14 19:40:49 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ResponseFiles.hpp"
#include "RequestParser.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <cstdlib>

#define PORT 8080

class Server
{
private:
  int                 _serverSocket;
  int                 _clientSocket;
  sockaddr_in         _serverAdress;
  const ResponseFiles _responses;
  RequestParser       _requests;

  void  handleRequest(std::map<std::string, std::string>& files);
public:
  Server(const ResponseFiles& responses);
  ~Server();

  void  serverLoop();
};
