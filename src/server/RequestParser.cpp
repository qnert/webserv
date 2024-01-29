/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:22:33 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/29 14:15:51 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/RequestParser.hpp"

RequestParser::RequestParser() {}

RequestParser::~RequestParser() {}

void  RequestParser::parseRequestBuffer(const std::string& buffer)
{
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
  if (getcwd(buff, sizeof(buff)) == NULL)
    throw std::runtime_error("Couldn't fine working directory!");
  this->_curr_dir = buff;
  this->_curr_dir.append("/");
}

void  RequestParser::cleanUp()
{
  this->_requestLines.clear();
  this->_requestType.clear();
  this->_uri.clear();
  this->_host.clear();
}

const std::string& RequestParser::getRequestType() const {return (this->_requestType);}

const std::string& RequestParser::getUri() const {return (this->_uri);}

const std::string& RequestParser::getHost() const {return (this->_host);}

const std::string& RequestParser::getBody() const {return (this->_body);}

const std::string& RequestParser::getCurrdir() const {return (this->_curr_dir);}
