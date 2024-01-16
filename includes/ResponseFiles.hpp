/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseFiles.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:23 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/16 09:59:23 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "./MIME_type.hpp"

class MIME_type;

class ResponseFiles
{
private:
  std::map<std::string, std::string> _responseFiles; // stores response files with name as key and content as value
public:
  ResponseFiles();
  ~ResponseFiles();

  const std::map<std::string, std::string>& getResponseFiles() const;

  void  storeFileIntoMap(const std::string& name, const std::string& path, MIME_type data);
};
