/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:22:44 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/14 19:57:31 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

class RequestParser
{
private:
  std::vector<std::string> _requestLines;
  std::string              _requestType;
  std::string              _uri;
  std::string              _host;
public:
  RequestParser();
  ~RequestParser();

  void  parseRequestBuffer(const char* buffer);
  void  cleanUp();

  const std::string& getRequestType() const;
  const std::string& getUri() const;
  const std::string& getHost() const;
};
