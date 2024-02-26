/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:22:44 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/26 11:04:27 by njantsch         ###   ########.fr       */
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
#include <ctime>

#define RECV_BUFFER_SIZE 10000

class RequestParser
{
private:
  std::map<std::string, std::string> _requestFields;
  std::string                        _buffer;
  std::string                        _curr_dir;
  std::string                        _fileType;
  std::string                        _boundary;
  bool                               _pendingReceive;
  ssize_t                            _totalReadBytes;
  std::string                        _redirectURL;
  std::string                        _indexFile;
  std::string                        _locIndexFile;

  void  parseRequestBody(const std::string& buffer);
  void  parseRequestHeader(const std::string& buffer);
public:
  RequestParser();
  ~RequestParser();

  void  parseRequestBuffer(const std::string& buffer, ssize_t bytes);
  void  cleanUp();

  void               setRedirect(std::string redir);
  void               setCurrDir(std::string currDir);
  void               setIndexFile(std::string index);
  void               setLocIndexFile(std::string index);

  bool               getPendingReceive() const;
  size_t             getBodySize();
  const std::string  getMapValue(const std::string key);
  const std::string& getFileType() const;
  const std::string& getBoundary() const;
  const std::string& getRequestType();
  const std::string& getUri();
  const std::string& getHost();
  const std::string& getBody();
  const std::string& getCurrdir();
  const std::string& getIndexFile();
  const std::string& getRedirectURL();
  const std::string& getLocIndexFile();
};
