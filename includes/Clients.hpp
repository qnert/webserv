/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 14:31:17 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/23 15:44:22 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "RequestParser.hpp"

typedef enum e_FdStatus
{
  CLOSE,
  KEEPALIVE,
  UNUSED,
  USED
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
public:
  Clients();
  ~Clients();

  void    setConStatus(int status);
  void    setFdStatus(int status);
  void    setError(bool status);
  void    storeHeaderSize(size_t headerSize);
  void    storeBufferSize(size_t bufferSize);
  void    checkPendingResponse(size_t bytesSend);
  void    cleanUpResponse();

  bool    getPendingResponse() const;
  ssize_t getTotalBytesSend() const;
  size_t  getHeaderSize() const;
  int     getConStatus() const;
  int     getFdStatus() const;
  bool    isError() const;
};
