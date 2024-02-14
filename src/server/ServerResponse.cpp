/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 18:33:28 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/14 16:06:01 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void  Server::getMethod(size_t idx, std::string& tmp)
{
  std::string uri = this->_clientDetails[idx].getUri();
  std::string msg = storeFileIntoString(this->_clientDetails[idx], uri);
  const std::string requestType = this->_clientDetails[idx].getRequestType();

  if (!msg.empty())
  {
    std::string length = Server::ft_itos(msg.size());
    std::string response = check_and_add_header(200, this->_data[requestType], length, this->_codes, this->_clientDetails[idx]) + msg;
    send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
  }
  else if (uri.find("/?searchTerm=") != std::string::npos) // searching for file in url if it's already there
  {
    size_t start = uri.find("/?searchTerm=") + 13;
    size_t end = uri.size();
    std::string filename = uri.substr(start, end - start);
    if (tmp == filename && filename.size() > 0)
    {
      msg = storeFileIntoString(this->_clientDetails[idx], "responseFiles/erased.html");
      std::string length = Server::ft_itos(msg.size());
      std::string response = check_and_add_header(200, this->_data["html"], length, this->_codes, this->_clientDetails[idx]) + msg;
      send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
    }
    else
      this->NotFound(idx);
  }
  else
    this->NotFound(idx);
}

int  Server::postMethod(size_t idx)
{
  std::string uri = this->_clientDetails[idx].getUri();
  if (uri == "/responseFiles/cgi-bin/first.cgi")
  {
    pid_t pid = fork();
    if (pid == 0)
      handle_name_input(this->_clientPollfds[idx].fd, this->_clientDetails[idx]);
    waitpid(0, NULL, 0);
    return (0);
  }
  else if (uri == "/responseFiles/cgi-bin/cpp_uploadfile.cgi"){
    handle_file_upload(this->_clientPollfds[idx].fd, this->_clientDetails[idx], this->_data, this->_codes);
    return (0);
  }
  else{
    CGI(this->_clientPollfds[idx].fd, uri, this->_clientDetails[idx].getBody());
    return (0);
  }
  return (1);
}

void  Server::notImplemented(size_t idx)
{
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "responseFiles/error501.html");
  std::string length = Server::ft_itos(msg.size());
  std::string contentType = this->_data[this->_clientDetails[idx].getRequestType()];
  std::string response = check_and_add_header(501, contentType, length, this->_codes, this->_clientDetails[idx]) + msg;
  send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
}

void  Server::methodNotAllowed(size_t idx)
{
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "responseFiles/error405.html");
  std::string length = Server::ft_itos(msg.size());
  std::string contentType = this->_data[this->_clientDetails[idx].getRequestType()];
  std::string response = check_and_add_header(405, contentType, length, this->_codes, this->_clientDetails[idx]) + msg;
  send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
}

void  Server::NotFound(size_t idx)
{
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "responseFiles/error404.html");
  std::string length = Server::ft_itos(msg.size());
  std::string contentType = this->_data[this->_clientDetails[idx].getRequestType()];
  std::string response = check_and_add_header(404, contentType, length, this->_codes, this->_clientDetails[idx]) + msg;
  send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
}
