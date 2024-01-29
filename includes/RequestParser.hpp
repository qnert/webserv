/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:22:44 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/29 16:52:14 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <climits>

class RequestParser
{
private:
  std::vector<std::string> _requestLines;
  std::string              _requestType;
  std::string              _uri;
  std::string              _host;
  std::string              _body;
  std::string              _curr_dir;
  bool                     _refreshed;

public:
  RequestParser();
  ~RequestParser();

  void  parseRequestBuffer(const std::string& buffer);
  void  cleanUp();

  const std::string& getRequestType() const;
  const std::string& getUri() const;
  const std::string& getHost() const;
  const std::string& getBody() const;
  const std::string& getCurrdir() const;
  bool               getRefreshed() const;
};
