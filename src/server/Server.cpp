#include "Server.hpp"

const int&  Server::getFd() const{
  return (this->server_fd);
}

Server::Server(struct sockaddr_in address){
  if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    throw(std::runtime_error("Error: socket function failure\n"));
  }
  if (bind(this->server_fd, (struct sockaddr *) &address, sizeof(address)) < 0){
    throw(std::runtime_error("Error: bind function failure\n"));
  }
  if (listen(this->server_fd, 3) < 0){
    throw(std::runtime_error("Error: listen function failure\n"));
  }
}

Server::~Server(){
  close(this->server_fd);
}

