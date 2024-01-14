/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:22:33 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/14 18:56:15 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/RequestParser.hpp"

RequestParser::RequestParser() {}

RequestParser::~RequestParser() {}

void  RequestParser::parseRequestBuffer(const char* buffer)
{
  std::istringstream bufferStream(buffer);
  std::string token;

  while (std::getline(bufferStream, token, '\n'))
    this->_requestLines.push_back(token);

  std::istringstream typeAndUri(this->_requestLines[0]);
  typeAndUri >> this->_requestType >> this->_uri;

  std::istringstream lineStreamForHost(this->_requestLines[1]);
  lineStreamForHost >> token >> this->_host;

  std::cout << this->_requestType << '\n' << this->_uri << '\n' << this->_host << std::endl;
}
