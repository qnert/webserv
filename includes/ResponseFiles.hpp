/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseFiles.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:23 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/16 11:24:57 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "./MIME_type.hpp"
#include "./Statuscodes.hpp"

class MIME_type;
class Statuscodes;

class ResponseFiles
{
private:
  std::map<std::string, std::string> _responseFiles; // stores response files with name as key and content as value
public:
  ResponseFiles();
  ~ResponseFiles();

  const std::map<std::string, std::string>& getResponseFiles() const;

  void  storeFileIntoMap(const std::string& name, const std::string& path, MIME_type data, Statuscodes codes);
};
