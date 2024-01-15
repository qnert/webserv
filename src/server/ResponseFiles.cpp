/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseFiles.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:09:55 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/15 13:53:29 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ResponseFiles.hpp"

std::string get_type(std::string const& type){
  if (type == "html")
    return ("text/html");
  if (type == "webp")
    return ("image/webp");
  if (type == "gif")
    return ("image/gif");
  if (type == "mp4")
    return ("video/mp4");
  return ("");
}

std::string  check_and_add_header(std::string const& type, bool is_valid){
  std::ostringstream header;
  if (is_valid == true)
    header << "HTTP/1.1 200 OK\r\n";
  else
    header << "HTTP/1.1 404 Not Found\r\n";
  header << "Content-Type: "<< get_type(type) << "\r\n";
  header << "\r\n";
  return (header.str());
}

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
    fileContent = check_and_add_header(path.substr(path.find_last_of(".") + 1), is_valid) + fileContent;
    this->_responseFiles.insert(std::pair<std::string, std::string>(name, fileContent));
  }
  file.close();
}
