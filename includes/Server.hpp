/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:16 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/23 10:49:38 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ResponseFiles.hpp"
#include "RequestParser.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
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
  RequestParser       _requests;

  void  handleRequest(RequestParser req, MIME_type data, Statuscodes codes);
public:
  Server();
  ~Server();

  void  serverLoop(MIME_type data, Statuscodes codes);
};
