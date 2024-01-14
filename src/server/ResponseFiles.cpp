/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseFiles.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:09:55 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/14 16:15:08 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ResponseFiles.hpp"

ResponseFiles::ResponseFiles() {}

ResponseFiles::~ResponseFiles() {}

const std::map<std::string, std::string>& ResponseFiles::getResponseFiles() const
{
  return (this->_responseFiles);
}

void  ResponseFiles::storeFileIntoMap(const std::string& name, const std::string& path)
{
  std::ifstream file(path, std::ios::binary);
  if (!file.is_open())
    throw(std::runtime_error("couldn't open file in storeFileIntoMap"));

  std::map<std::string, std::string>::iterator it = this->_responseFiles.find(name);
  if (it == this->_responseFiles.end())
  {
    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string fileContent = buffer.str();
    this->_responseFiles.insert(std::pair<std::string, std::string>(name, fileContent));
  }
  file.close();
}
