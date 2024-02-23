/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:22:33 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/23 20:51:15 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/RequestParser.hpp"

RequestParser::RequestParser() : _pendingReceive(false), _totalReadBytes(0) {}

RequestParser::~RequestParser() {}

void  RequestParser::parseRequestHeader(const std::string& buffer)
{
  std::istringstream bufferStream(buffer);
  std::string line;
  size_t headerLength = 0;

  std::getline(bufferStream, line, '\n');

  headerLength += line.size() + 1;
  std::istringstream tmp(line);
  tmp >> this->_requestFields["Type"] \
    >> this->_requestFields["Uri"] >> this->_requestFields["Version"];

  while (std::getline(bufferStream, line, '\n'))
  {
    if (line == "\r") {
      headerLength += 2;
      this->_totalReadBytes -= headerLength;
      break ;
    }
    size_t pos = line.find(':');
    if (pos != std::string::npos)
    {
      std::string key = line.substr(0, pos);
      std::string value = line.substr(pos + 2);
      value.erase(value.end() - 1);
      this->_requestFields[key] = value;
    }
    headerLength += line.size() + 1;
  }
}

void  RequestParser::parseRequestBody(const std::string& buffer)
{
  size_t it = buffer.find("\r\n\r\n");
  if (it == buffer.size()) {
    this->_requestFields["Body"] = "";
  }
  else
    this->_requestFields["Body"] = buffer.substr(it + 4);
  if (this->_requestFields["Uri"] == "/" || this->_requestFields["Type"] == "POST"
        || this->_requestFields["Type"] == "DELETE")
    this->_fileType = "html";
  else {
    std::string uri = this->_requestFields["Uri"];
    this->_fileType = uri.substr(uri.find_last_of('.') + 1, uri.size() - uri.find_last_of('.'));
  }
  std::string content_type = this->_requestFields["Content-Type"];
  size_t  start_bound = content_type.find("boundary=");
  if (start_bound == std::string::npos)
    this->_boundary = "";
  else
    this->_boundary = content_type.substr(start_bound + 9, content_type.length() - start_bound + 9);
}

void  RequestParser::parseRequestBuffer(const std::string& buffer, ssize_t bytes)
{
  this->_totalReadBytes += bytes;
  if (!this->_pendingReceive) {
    parseRequestHeader(buffer);
    this->_buffer = buffer;
  }
  else
    this->_buffer.append(buffer);
  if (std::atoi(this->_requestFields["Content-Length"].c_str()) <= this->_totalReadBytes)
  {
    parseRequestBody(this->_buffer);
    this->_totalReadBytes = 0;
    this->_buffer.clear();
    this->_pendingReceive = false;
    return ;
  }
  this->_pendingReceive = true;
}

void  RequestParser::cleanUp()
{
  this->_boundary.clear();
  this->_fileType.clear();
  this->_requestFields.clear();
}

void  RequestParser::setCurrDir(std::string currDir) {this->_curr_dir = currDir + "/";}

bool  RequestParser::getPendingReceive() const {return (this->_pendingReceive);}

const std::string& RequestParser::getRequestType() {return (this->_requestFields["Type"]);}

const std::string& RequestParser::getUri() {return (this->_requestFields["Uri"]);}

const std::string& RequestParser::getHost() {return (this->_requestFields["Host"]);}

const std::string& RequestParser::getBody() {return (this->_requestFields["Body"]);}

const std::string& RequestParser::getCurrdir() {return (this->_curr_dir);}

const std::string& RequestParser::getFileType() const {return (this->_fileType);}

const std::string& RequestParser::getBoundary() const{return (this->_boundary);}

size_t  RequestParser::getBodySize() {return (this->_requestFields["Body"].size());}

const std::string RequestParser::getMapValue(const std::string key)
{
  if (this->_requestFields.count(key) > 0)
    return (this->_requestFields[key]);
  return ("");
}

