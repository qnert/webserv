/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:22:44 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/11 18:07:46 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <unistd.h>
#include <climits>

#define RECV_BUFFER_SIZE 10000

class RequestParser
{
private:
  std::map<std::string, std::string> _requestFields;
  std::string                        _buffer;
  std::string                        _curr_dir;
  std::string                        _fileType;
  bool                               _status;
  bool                               _pendingReceive;
  int                                _totalReadBytes;

  void  parseRequestBody(const std::string& buffer);
  void  parseRequestHeader(const std::string& buffer);
public:
  RequestParser();
  ~RequestParser();

  void  parseRequestBuffer(const std::string& buffer, ssize_t bytes);
  void  reset_status();
  void  cleanUp();

  bool               getPendingReceive() const;
  const std::string& getFileType() const;
  const std::string& getRequestType();
  const std::string& getUri();
  const std::string& getHost();
  const std::string& getBody();
  const std::string& getCurrdir();
  const std::string  getMapValue(const std::string key);
};
