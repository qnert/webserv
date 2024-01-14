/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:16 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/14 15:44:35 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ResponseFiles.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <stdio.h>

#define PORT 8080

class Server
{
private:
  int _serverSocket;
  int _clientSocket;
  sockaddr_in _serverAdress;
  const ResponseFiles _responses;

  void  handleRequest(std::map<std::string, std::string>& files);
public:
  Server(const ResponseFiles& responses);
  ~Server();

  void  serverLoop();
};
