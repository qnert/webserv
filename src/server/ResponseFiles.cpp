/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseFiles.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:09:55 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/16 11:48:55 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ResponseFiles.hpp"

ResponseFiles::ResponseFiles() {}

ResponseFiles::~ResponseFiles() {}

const std::map<std::string, std::string>& ResponseFiles::getResponseFiles() const
{
  return (this->_responseFiles);
}

void  ResponseFiles::storeFileIntoMap(const std::string& name, const std::string& path)
{
  bool  is_valid = true;
  std::ifstream file(path, std::ios::binary);
  if (!file.is_open())
    is_valid = false;

  std::map<std::string, std::string>::iterator it = this->_responseFiles.find(name);
  if (it == this->_responseFiles.end())
  {
    std::string fileContent;
    if (is_valid == true){
      std::ostringstream buffer;
      buffer << file.rdbuf();
      fileContent = buffer.str();
    }
    this->_responseFiles.insert(std::pair<std::string, std::string>(name, fileContent));
  }
  file.close();
}
