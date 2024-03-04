/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseMethods.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 15:05:57 by njantsch          #+#    #+#             */
/*   Updated: 2024/03/04 17:24:32 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void  Server::getMethod(size_t idx, std::string& tmp)
{
  DIR*  check;
  std::string root = this->_clientDetails[idx].getCurrdir();
  std::string uri = this->_clientDetails[idx].getUri();
  check = opendir((root + uri).c_str());
  getRightIndexFile(idx);
  std::string msg = storeFileIntoString(this->_clientDetails[idx], uri);

  if (check != NULL && !this->_currLocation.empty() && this->_currLocation["autoindex"] == "on") {
    list_directories(this->_clientPollfds[idx].fd, this->_clientDetails[idx], this->_codes, check);
    check = NULL;
  }
  else if (!msg.empty()){
      this->handleGetDefault(msg, idx);
  }
  else if (uri.find("/?searchTerm=") == std::string::npos && uri.find("?") != std::string::npos){
    try{
      CGI(this->_clientPollfds[idx].fd, uri, this->_clientDetails[idx].getBody(), this->_clientDetails[idx].getCurrdir(), "GET", *this);
    }
    catch (const std::exception& e){
      this->_clientDetails[idx].setConStatus(CLOSE);
    }
  }
  else if (uri.find("/?searchTerm=") != std::string::npos) // searching for file in url if it's already there
  {
    size_t start = uri.find("/?searchTerm=") + 13;
    size_t end = uri.size();
    std::string filename = uri.substr(start, end - start);
    if (tmp == filename && filename.size() > 0)
    {
      msg = storeFileIntoString(this->_clientDetails[idx], "/erased.html");
      std::string length = ft_itos(msg.size());
      std::string response = check_and_add_header(200, this->_data["html"], length, this->_codes, this->_clientDetails[idx]) + msg;
      if (send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0) < 0)
        this->_clientDetails[idx].setConStatus(CLOSE);
    }
    else if (checkLocationPrelims("DELETE") == true)
      this->methodNotAllowed(idx);
    else
      this->NotFound(idx);
  }
  else
    this->NotFound(idx);
  if (check != NULL)
    closedir(check);
}

void  Server::handleGetDefault(std::string& msg, size_t idx)
{
  std::string response;
  const std::string fileType = this->_clientDetails[idx].getFileType();

  if (this->_clientDetails[idx].getPendingResponse() == false) {
    std::string length = ft_itos(msg.size());
    response = check_and_add_header(200, this->_data[fileType], length, this->_codes, this->_clientDetails[idx]);
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

int  Server::postMethod(size_t idx)
{
  std::string uri = this->_clientDetails[idx].getUri();
  if (uri == "/cgi-bin/first.cgi")
  {
    pid_t pid = fork();
    if (pid == 0)
      handle_name_input(this->_clientPollfds[idx].fd, this->_clientDetails[idx]);
    waitpid(0, NULL, 0);
    return (0);
  }
  else if (uri == "/cgi-bin/cpp_uploadfile.cgi"){
    handle_file_upload(this->_clientPollfds[idx].fd, this->_clientDetails[idx], this->_data, this->_codes);
    return (0);
  }
  else{
    try{
      CGI(this->_clientPollfds[idx].fd, uri, this->_clientDetails[idx].getBody(), this->_clientDetails[idx].getCurrdir(), "POST", *this);
    }
    catch (const std::exception& e){
      this->_clientDetails[idx].setConStatus(CLOSE);
    }
    return (0);
  }
  return (1);
}
