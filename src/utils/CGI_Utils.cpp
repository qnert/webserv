/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI_Utils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:05:00 by skunert           #+#    #+#             */
/*   Updated: 2024/02/26 16:21:16 by rnauke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CGI.hpp"
#include "../../includes/Server.hpp"

std::string get_exec_type(std::string const& file_path){
  size_t  start = file_path.find_last_of('.');
  if (start == std::string::npos)
    return ("");
  size_t  end = file_path.length();
  std::string file = file_path.substr(start, end - start);
  size_t prefix = file.find_last_of('.');
  if (prefix == std::string::npos)
    return ("");
  return (file.substr(prefix, file.size() - prefix));
}

std::string check_exec_type(std::string const& exec_type){
  if (exec_type == ".py"){
    return ("/usr/local/bin/python3");
  }
  else if (exec_type == ".pl"){
    return ("/usr/bin/perl");
  }
  return ("");
}

std::string  add_header_cgi(int status, Statuscodes& codes)
{
  std::ostringstream header;
  header << "HTTP/1.1 " << status << " " << codes[status];
  return (header.str());
}

std::string  storeFileIntoString_cgi(std::string path)
{
  std::ifstream file(path.c_str(), std::ios::binary);
  if (!file.is_open())
    return ("");

  std::string fileContent;
  std::ostringstream buffer;
  buffer << file.rdbuf();
  fileContent = buffer.str();
  file.close();
  return (fileContent);
}

void  get_path_info_get(std::string&  exec_name, std::string& path_info, std::string& body){
  size_t  start = exec_name.find("/cgi-bin/");
  if (start == std::string::npos){
    exec_name = "";
    return ;
  }
  size_t  end = exec_name.find_first_of('?', start);
  size_t  body_start = exec_name.find_first_of('/', end + 1);
  if (exec_name.find('?', end + 1) != std::string::npos){
    exec_name = "";
    return ;
  }
  if (body_start == std::string::npos)
    path_info = "";
  else
    path_info = exec_name.substr(body_start, exec_name.length() - body_start);
  body = exec_name.substr(end + 1, body_start - end - 1);
  exec_name = exec_name.substr(start, end - start);
}

void  get_path_info_post(std::string&  exec_name, std::string& path_info){
  size_t start = exec_name.find("/cgi-bin/");
  std::string path = exec_name.substr(start + 9, exec_name.length() - start + 22);
  start = path.find_first_of('/');
  if (start == std::string::npos){
    exec_name = exec_name.substr(0, exec_name.length());
    path_info = "/";
    return ;
  }
  else{
    exec_name = "/cgi-bin/" + path.substr(0, start);
    path_info = path.substr(start, path_info.length() - start + 1);
    return ;
  }
}
