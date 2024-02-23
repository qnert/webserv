/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:19:30 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/23 15:44:46 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Clients.hpp"

Clients::Clients() : RequestParser(), _pendingResponse(false), _totalBytesSend(0),
                  _totalBufferSize(0), _headerSize(0), _connectionStatus(KEEPALIVE),
                  _fdStatus(UNUSED), _error(false) {}

Clients::~Clients() {}

bool  Clients::getPendingResponse() const {return (this->_pendingResponse);}

ssize_t Clients::getTotalBytesSend() const {return (this->_totalBytesSend);}

size_t Clients::getHeaderSize() const {return (this->_headerSize);}

int    Clients::getConStatus() const {return (this->_connectionStatus);}

int    Clients::getFdStatus() const {return (this->_fdStatus);}

bool   Clients::isError() const {return (this->_error);}

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

void  Clients::setError(bool status) {this->_error = status;}

void  Clients::setConStatus(int status) {this->_connectionStatus = status;}

void  Clients::setFdStatus(int status) {this->_fdStatus = status;}

void  Clients::cleanUpResponse()
{
  this->_headerSize = 0;
  this->_pendingResponse = false;
  this->_totalBufferSize = 0;
  this->_totalBytesSend = 0;
  this->_error = false;
}
