/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:22:33 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/16 18:03:45 by skunert          ###   ########.fr       */
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
