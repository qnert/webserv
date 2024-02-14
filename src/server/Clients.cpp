/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:19:30 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/14 17:44:27 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Clients.hpp"

Clients::Clients() : RequestParser(), _pendingResponse(false), _totalBytesSend(0) {}

Clients::~Clients() {}

bool  Clients::getPendingResponse() const {return (this->_pendingResponse);}

ssize_t Clients::getTotalBytesSend() const {return (this->_totalBytesSend);}

size_t Clients::getHeaderSize() const {return (this->_headerSize);}

void  Clients::checkPendingResponse(size_t bytesSend)
{
  this->_totalBytesSend += bytesSend;
  if (this->_totalBufferSize <= this->_totalBytesSend)
    this->_pendingResponse = false;
  else
    this->_pendingResponse = true;
}

void  Clients::storeHeaderSize(size_t headerSize)
{
  this->_headerSize = headerSize;
}

void  Clients::storeBufferSize(size_t bufferSize)
{
  this->_totalBufferSize = bufferSize;
}

void  Clients::cleanUpResponse()
{
  this->_totalBufferSize = 0;
  this->_totalBytesSend = 0;
}
