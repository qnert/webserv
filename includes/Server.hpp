#pragma once

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
  int serverSocket;
  int clientSocket;
  sockaddr_in serverAdress;
public:
  Server();
  ~Server();

  void  serverLoop();
};
