#include "./server/Server.hpp"

int main(int argc, char **argv){
  if (argc > 2 && argv != NULL)
    return (std::cerr << "False amount of arguments!" << std::endl, 1);
  int new_socket;
  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(0x7f000001);
  address.sin_port = htons(8080);
  int addrlen = sizeof(address);
  Server server(address);
  while (1){
    if ((new_socket = accept(server.getFd(), (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0){
        return (std::cerr << "Error: accept function failed!" << std::endl, 1);
      }
    if (send(new_socket, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\t\t<meta charset=\"UTF-8\">\n\t\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\t\t<title>Hello World</title>\n</head>\n<body>\n\t\t<h1>Hello, World!</h1>\n</body>\n</html>", 241, 0) == -1){
      return (std::cerr << "Error: send function failed!" << std::endl, 1);
    }
  }
}
