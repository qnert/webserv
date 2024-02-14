/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 14:31:17 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/14 16:22:17 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "RequestParser.hpp"
#include <string>

class Clients : public RequestParser
{
private:
  bool         _pendingResponse;
  size_t       _totalBytesSend;
  size_t       _totalBufferSize;
  size_t       _headerSize;
public:
  Clients();
  ~Clients();

  void    storeHeaderSize(size_t headerSize);
  void    storeBufferSize(size_t bufferSize);
  void    checkPendingResponse(size_t bytesSend);
  void    cleanUpResponse();

  bool    getPendingResponse() const;
  ssize_t getTotalBytesSend() const;
  size_t  getHeaderSize() const;
};
