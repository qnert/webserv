/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:22:33 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/10 17:52:33 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/RequestParser.hpp"

RequestParser::RequestParser() {}

RequestParser::~RequestParser() {}

void  RequestParser::parseRequestBuffer(const std::string& buffer)
{
  static int i;

  if (i == 1){
    this->_requestFields["Uri"] = "upload";
    this->_requestFields["Body"] = buffer;
    i = 0;
    return ;
  }

  std::istringstream bufferStream(buffer);
  std::string line;

  bufferStream >> this->_requestFields["Type"] \
    >> this->_requestFields["Uri"] >> this->_requestFields["Version"];

  for (int j = 0; std::getline(bufferStream, line, '\n'); j++)
  {
    size_t pos = line.find(':');
    if (pos != std::string::npos)
    {
      std::string key = line.substr(0, pos);
      std::string value = line.substr(pos + 2);
      value.erase(value.end() - 1);
      this->_requestFields[key] = value;
    }
    else if (j != 0)
      break ;
  }

  size_t it = buffer.find_last_of("\n\n");
  if (it == buffer.size())
    this->_requestFields["Body"] = "";
  else
    this->_requestFields["Body"] = buffer.substr(it + 1, buffer.size());
  char buff[PATH_MAX];
  if (getcwd(buff, sizeof(buff)) == NULL)
    throw std::runtime_error("Couldn't fine working directory!");
  this->_curr_dir = buff;
  this->_curr_dir.append("/");
  if (this->_requestFields["Uri"] == "/" || this->_requestFields["Type"] == "POST"
        || this->_requestFields["Type"] == "DELETE")
    this->_fileType = "html";
  else
    this->_fileType = this->_requestFields["Uri"].substr(this->_requestFields["Uri"].find_last_of('.') + 1, this->_requestFields["Uri"].size() - this->_requestFields["Uri"].find_last_of('.'));
  if (this->_requestFields["Uri"] == "/responseFiles/cpp_uploadfile.cgi")
    i = 1;
}

void  RequestParser::cleanUp()
{
  this->_fileType.clear();
  this->_requestFields.clear();
}

const std::string& RequestParser::getRequestType() {return (this->_requestFields["Type"]);}

const std::string& RequestParser::getUri() {return (this->_requestFields["Uri"]);}

const std::string& RequestParser::getHost() {return (this->_requestFields["Host"]);}

const std::string& RequestParser::getBody() {return (this->_requestFields["Body"]);}

const std::string& RequestParser::getCurrdir() {return (this->_curr_dir);}

const std::string& RequestParser::getFileType() const {return (this->_fileType);}

const std::string RequestParser::getMapValue(const std::string key)
{
  if (this->_requestFields.count(key) > 0)
    return (this->_requestFields[key]);
  return ("");
}

