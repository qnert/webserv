/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseFiles.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:09:55 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/16 11:26:39 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ResponseFiles.hpp"

std::string  check_and_add_header(std::string const& type, bool is_valid, MIME_type data, Statuscodes codes){
  std::ostringstream header;
  if (is_valid == true)
    header << "HTTP/1.1 200 " << codes[200] << "\r\n";
  else
    header << "HTTP/1.1 404 " << codes[404] << "\r\n";
  header << "Content-Type: "<< data[type] << "\r\n";
  header << "\r\n";
  return (header.str());
}

ResponseFiles::ResponseFiles() {}

ResponseFiles::~ResponseFiles() {}

const std::map<std::string, std::string>& ResponseFiles::getResponseFiles() const
{
  return (this->_responseFiles);
}

void  ResponseFiles::storeFileIntoMap(const std::string& name, const std::string& path, MIME_type data, Statuscodes codes)
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
    fileContent = check_and_add_header(path.substr(path.find_last_of(".") + 1), is_valid, data, codes) + fileContent;
    this->_responseFiles.insert(std::pair<std::string, std::string>(name, fileContent));
  }
  file.close();
}
