/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 18:33:28 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/08 17:39:11 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void  Server::getMethod(MIME_type& data, Statuscodes& codes, size_t idx, std::string& tmp)
{
  std::string uri = this->_requests.getUri();
  std::string msg = storeFileIntoString(this->_requests, uri);
  const std::string requestType = this->_requests.getRequestType();

  if (!msg.empty())
  {
    std::string length = Server::ft_itos(msg.size());
    std::string response = check_and_add_header(200, data[requestType], length, codes) + msg;
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
      std::string response = check_and_add_header(200, data["html"], length, codes) + msg;
      send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
    }
    else
    {
      msg = storeFileIntoString(this->_requests, "responseFiles/error404.html");
      std::string length = Server::ft_itos(msg.size());
      std::string response = check_and_add_header(404, data["html"], length, codes) + msg;
      send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
    }
  }
  else
  {
    std::string errorMsg = storeFileIntoString(this->_requests, "responseFiles/error404.html");
    std::string length = Server::ft_itos(errorMsg.size());
    std::string response = check_and_add_header(404, data["html"], length, codes) + errorMsg;
    send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
  }
}

void  Server::postMethod(MIME_type& data, Statuscodes& codes, size_t idx)
{
  std::string uri = this->_requests.getUri();
  if (uri == "/responseFiles/first.cgi")
  {
    pid_t pid = fork();
    if (pid == 0)
        handle_Request_post(this->_clientPollfds[idx].fd, this->_requests, data, codes);
    waitpid(0, NULL, 0);
  }
  else if (uri == "upload" || uri == "/responseFiles/cpp_fileupload.cgi"){
    handle_Request_post(this->_clientPollfds[idx].fd, this->_requests, data, codes);
  }
}

void  Server::notImplemented(MIME_type& data, Statuscodes& codes, size_t idx)
{
  std::string msg = storeFileIntoString(this->_requests, "responseFiles/error501.html");
  std::string length = Server::ft_itos(msg.size());
  std::string response = check_and_add_header(501, data[this->_requests.getRequestType()], length, codes) + msg;
  send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
}

void  Server::methodNotAllowed(MIME_type& data, Statuscodes& codes, size_t idx)
{
  std::string msg = storeFileIntoString(this->_requests, "responseFiles/error405.html");
  std::string length = Server::ft_itos(msg.size());
  std::string response = check_and_add_header(405, data[this->_requests.getRequestType()], length, codes) + msg;
  send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
}
