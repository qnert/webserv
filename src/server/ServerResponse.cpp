/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 18:33:28 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/10 17:16:15 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void  Server::getMethod(size_t idx, std::string& tmp)
{
  std::string uri = this->_requests.getUri();
  std::string msg = storeFileIntoString(this->_requests, uri);
  const std::string requestType = this->_requests.getRequestType();

  if (!msg.empty())
  {
    std::string length = Server::ft_itos(msg.size());
    std::string response = check_and_add_header(200, this->_data[requestType], length, this->_codes, this->_requests) + msg;
    send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
  }
  else if (uri.find("/?searchTerm=") != std::string::npos) // searching for file in url if it's already there
  {
    size_t start = uri.find("/?searchTerm=") + 13;
    size_t end = uri.size();
    std::string filename = uri.substr(start, end - start);
    if (tmp == filename)
    {
      msg = storeFileIntoString(this->_requests, "responseFiles/erased.html");
      std::string length = Server::ft_itos(msg.size());
      std::string response = check_and_add_header(200, this->_data["html"], length, this->_codes, this->_requests) + msg;
      send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
    }
    else
    {
      msg = storeFileIntoString(this->_requests, "responseFiles/error404.html");
      std::string length = Server::ft_itos(msg.size());
      std::string response = check_and_add_header(404, this->_data["html"], length, this->_codes, this->_requests) + msg;
      send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
    }
  }
  else
  {
    std::string errorMsg = storeFileIntoString(this->_requests, "responseFiles/error404.html");
    std::string length = Server::ft_itos(errorMsg.size());
    std::string response = check_and_add_header(404, this->_data["html"], length, this->_codes, this->_requests) + errorMsg;
    send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
  }
}

int  Server::postMethod(size_t idx)
{
  std::string uri = this->_requests.getUri();
  if (uri == "/responseFiles/first.cgi")
  {
    pid_t pid = fork();
    if (pid == 0)
        handle_Request_post(this->_clientPollfds[idx].fd, this->_requests, this->_data, this->_codes);
    waitpid(0, NULL, 0);
    return (0);
  }
  else if (uri == "upload" || uri == "/responseFiles/cpp_fileupload.cgi"){
    handle_Request_post(this->_clientPollfds[idx].fd, this->_requests, this->_data, this->_codes);
    return (0);
  }
  return (1);
}

void  Server::notImplemented(size_t idx)
{
  std::string msg = storeFileIntoString(this->_requests, "responseFiles/error501.html");
  std::string length = Server::ft_itos(msg.size());
  std::string contentType = this->_data[this->_requests.getRequestType()];
  std::string response = check_and_add_header(501, contentType, length, this->_codes, this->_requests) + msg;
  send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
}

void  Server::methodNotAllowed(size_t idx)
{
  std::string msg = storeFileIntoString(this->_requests, "responseFiles/error405.html");
  std::string length = Server::ft_itos(msg.size());
  std::string contentType = this->_data[this->_requests.getRequestType()];
  std::string response = check_and_add_header(405, contentType, length, this->_codes, this->_requests) + msg;
  send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
}
