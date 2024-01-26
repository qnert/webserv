/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:22:44 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/23 13:23:25 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "unistd.h"

class RequestParser
{
private:
  std::vector<std::string> _requestLines;
  std::string              _requestType;
  std::string              _uri;
  std::string              _host;
  std::string              _body;
  std::string              _curr_dir;

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
};
