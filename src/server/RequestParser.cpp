/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:22:33 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/02 11:19:20 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/RequestParser.hpp"

RequestParser::RequestParser() {}

RequestParser::~RequestParser() {}

void  RequestParser::parseRequestBuffer(const std::string& buffer)
{
  static int i;
  if (i == 1){
    this->_uri = "upload";
    this->_body = buffer;
    i = 0;
    return ;
  }
  std::istringstream bufferStream(buffer);
  std::string token;

  while (std::getline(bufferStream, token, '\n'))
    this->_requestLines.push_back(token);

  std::istringstream typeAndUri(this->_requestLines[0]);
  typeAndUri >> this->_requestType >> this->_uri;

  std::istringstream lineStreamForHost(this->_requestLines[1]);
  lineStreamForHost >> token >> this->_host;

  size_t it = buffer.find_last_of("\n\n");
  if (it == buffer.size())
    this->_body = "";
  else
    this->_body = buffer.substr(it + 1, buffer.size());
  char buff[PATH_MAX];
  if (getcwd(buff, sizeof(buff)) == nullptr)
    throw std::runtime_error("Couldn't fine working directory!");
  this->_curr_dir = buff;
  this->_curr_dir.append("/");
  if (this->_uri == "/" || this->_requestType == "POST" || this->_requestType == "DELETE")
    this->_fileType = "html";
  else
    this->_fileType = this->_uri.substr(this->_uri.find_last_of('.') + 1, this->_uri.size() - this->_uri.find_last_of('.'));
  if (this->_uri == "/responseFiles/cpp_uploadfile.cgi")
    i = 1;
}

void  RequestParser::cleanUp()
{
  this->_requestLines.clear();
  this->_requestType.clear();
  this->_uri.clear();
  this->_host.clear();
  this->_fileType.clear();
}

const std::string& RequestParser::getRequestType() const {return (this->_requestType);}

const std::string& RequestParser::getUri() const {return (this->_uri);}

const std::string& RequestParser::getHost() const {return (this->_host);}

const std::string& RequestParser::getBody() const {return (this->_body);}

const std::string& RequestParser::getCurrdir() const {return (this->_curr_dir);}

const std::string& RequestParser::getFileType() const {return (this->_fileType);}
