/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileupload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 11:17:16 by skunert           #+#    #+#             */
/*   Updated: 2024/01/31 13:27:09 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <sstream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

std::string  storeFileIntoString(std::string path)
{
  std::ifstream file(path, std::ios::binary);
  if (!file.is_open())
    return ("");

  std::string fileContent;
  std::ostringstream buffer;
  buffer << "HTTP/1.1 200 OK\r\n";
  buffer << "Content-Type: text/html\r\n";
  buffer << "\r\n";
  buffer << file.rdbuf();
  fileContent = buffer.str();
  file.close();
  return (fileContent);
}

size_t  ft_strlen(const char* str){
  size_t len = 0;
  while (str[len])
    len++;
  return (len);
}

int main(int argc, char **argv){
  if (argc != 4)
    return (1);
  std::string path = argv[2];
  std::ofstream upload(("./responseFiles/Upload/" + path).c_str(), std::ios::binary);
  if (!upload.is_open())
    return (1);
  upload.write(argv[3], ft_strlen(argv[3]));
  upload.close();
  std::string msg = storeFileIntoString("/Users/skunert/Documents/webserv/responseFiles/success.html");
  if (msg != "")
    send(std::atoi(argv[1]), msg.c_str(), msg.size(), 0);
  return (0);
}
