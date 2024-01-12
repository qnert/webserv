#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <iostream>
#include <stdio.h>

class Server
{
private:
  int server_fd;

public:
  const int&  getFd() const;
  Server(struct sockaddr_in address);
  ~Server();
};
