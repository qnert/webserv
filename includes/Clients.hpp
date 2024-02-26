/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 14:31:17 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/25 19:42:57 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "RequestParser.hpp"

#define CLIENT_TIMEOUT 20

typedef enum e_FdStatus
{
  // connection status
  CLOSE,
  KEEPALIVE,
  // fd status
  UNUSED,
  USED,
  // socket types
  SERVER,
  CLIENT
} FdStatus;

class Clients : public RequestParser
{
private:
  bool         _pendingResponse;
  size_t       _totalBytesSend;
  size_t       _totalBufferSize;
  size_t       _headerSize;
  int          _connectionStatus;
  int          _fdStatus;
  bool         _error;
  time_t       _idleStartTime;
  int          _socketType;
public:
  Clients();
  ~Clients();

  void    setSocketType(int type);
  void    refreshTime(time_t refresh);
  void    setConStatus(int status);
  void    setFdStatus(int status);
  void    setError(bool status);
  void    storeHeaderSize(size_t headerSize);
  void    storeBufferSize(size_t bufferSize);
  void    checkPendingResponse(size_t bytesSend);
  void    cleanUpResponse();

  time_t  getTime() const;
  int     getSocketType() const;
  bool    getPendingResponse() const;
  ssize_t getTotalBytesSend() const;
  size_t  getHeaderSize() const;
  int     getConStatus() const;
  int     getFdStatus() const;
  bool    isError() const;
};
