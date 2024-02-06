/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:22:44 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/02 11:09:41 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <unistd.h>
#include <climits>

class RequestParser
{
private:
  std::map<std::string, std::string> _requestFields;
  std::string                        _curr_dir;
  std::string                        _fileType;

public:
  RequestParser();
  ~RequestParser();

  void  parseRequestBuffer(const std::string& buffer);
  void  cleanUp();

  const std::string& getFileType() const;
  const std::string& getRequestType();
  const std::string& getUri();
  const std::string& getHost();
  const std::string& getBody();
  const std::string& getCurrdir();
  const std::string getMapValue(const std::string key);
};
