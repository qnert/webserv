/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 18:33:28 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/23 13:03:09 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void  Server::getMethod(size_t idx, std::string& tmp)
{
  DIR*  check;
  std::string uri = this->_clientDetails[idx].getUri();
  check = opendir((uri.substr(1, uri.length())).c_str());
  std::string msg = storeFileIntoString(this->_clientDetails[idx], uri);

  if (!msg.empty())
    this->handleGetDefault(msg, idx);
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
      if (send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0) < 0)
        this->_clientDetails[idx].setConStatus(CLOSE);
    }
    else
      this->NotFound(idx);
  }
  else if (check != NULL)
    list_directories(this->_clientPollfds[idx].fd, this->_clientDetails[idx], this->_codes, check);
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
  this->_clientDetails[idx].setError(true);
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "responseFiles/error501.html");
  std::string length = Server::ft_itos(msg.size());
  std::string response = check_and_add_header(501, this->_data["html"], length, this->_codes, this->_clientDetails[idx]) + msg;
  if (send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0) < 0)
    this->_clientDetails[idx].setConStatus(CLOSE);
}

void  Server::methodNotAllowed(size_t idx)
{
  this->_clientDetails[idx].setError(true);
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "responseFiles/error405.html");
  std::string length = Server::ft_itos(msg.size());
  std::string response = check_and_add_header(405, this->_data["html"], length, this->_codes, this->_clientDetails[idx]) + msg;
  if (send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0) < 0)
    this->_clientDetails[idx].setConStatus(CLOSE);
}

void  Server::handleGetDefault(std::string& msg, size_t idx)
{
  std::string response;
  const std::string requestType = this->_clientDetails[idx].getRequestType();

  if (this->_clientDetails[idx].getPendingResponse() == false) {
    std::string length = Server::ft_itos(msg.size());
    response = check_and_add_header(200, this->_data[requestType], length, this->_codes, this->_clientDetails[idx]);
    this->_clientDetails[idx].storeHeaderSize(response.size());
    response += msg;
    this->_clientDetails[idx].storeBufferSize(response.size());
  }
  else {
    size_t totalBytesSend = this->_clientDetails[idx].getTotalBytesSend();
    size_t headerSize = this->_clientDetails[idx].getHeaderSize();
    response = msg.substr(totalBytesSend - headerSize);
  }
  ssize_t bytesSend = send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
  if (bytesSend == -1) {
    this->_clientDetails[idx].setConStatus(CLOSE);
    return;
  }
  this->_clientDetails[idx].checkPendingResponse(bytesSend);
}

void  Server::NotFound(size_t idx)
{
  this->_clientDetails[idx].setError(true);
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "responseFiles/error404.html");
  std::string length = Server::ft_itos(msg.size());
  std::string response = check_and_add_header(404, this->_data["html"], length, this->_codes, this->_clientDetails[idx]) + msg;
  if (send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0) < 0)
    this->_clientDetails[idx].setConStatus(CLOSE);
}

void  Server::versionNotSupported(size_t idx)
{
  this->_clientDetails[idx].setError(true);
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "responseFiles/error505.html");
  std::string length = Server::ft_itos(msg.size());
  std::string response = check_and_add_header(505, this->_data["html"], length, this->_codes, this->_clientDetails[idx]) + msg;
  if (send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0) < 0)
    this->_clientDetails[idx].setConStatus(CLOSE);
}
