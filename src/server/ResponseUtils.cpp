/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 18:16:25 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/29 18:49:37 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

std::string  storeFileIntoString(RequestParser& req, std::string path)
{
  if (req.getUri() == "/")
    path = req.getCurrdir() + "responseFiles/index.html";
  else
    path = req.getCurrdir() + path;
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

std::string get_first_name(std::string body)
{
  size_t  start_pos = body.find_first_of("=") + 1;
  size_t  end_pos = body.find_first_of("&") - body.find_first_of("=") - 1;
  std::string ret_str = body.substr(start_pos, end_pos);

  return (ret_str);
}

std::string get_last_name(std::string body)
{
  size_t  start_pos = body.find_last_of("=") + 1;
  size_t  end_pos = body.size();
  std::string ret_str = body.substr(start_pos, end_pos);

  return (ret_str);
}

void  handle_Request_post(int fd, RequestParser& req)
{
  char *argv[5];
  std::string cgi_filename = req.getUri().substr(req.getUri().find_last_of("/") + 1, req.getUri().size());
  std::string file_fd = Server::ft_itos(fd);
  std::string first_name = get_first_name(req.getBody());
  std::string last_name = get_last_name(req.getBody());

  argv[0] = const_cast<char*>(cgi_filename.c_str());
  argv[1] = const_cast<char*>(file_fd.c_str());
  argv[2] = const_cast<char*>(first_name.c_str());
  argv[3] = const_cast<char*>(last_name.c_str());
  argv[4] = NULL;
  execve((req.getCurrdir() + req.getUri()).c_str(), argv, NULL);
}

std::string  check_and_add_header(int status, std::string const& type, MIME_type& data, Statuscodes& codes)
{
  std::ostringstream header;
  header << "HTTP/1.1 " << status << " " << codes[status] << "\r\n";
  header << "Content-Type: "<< data[type] << "\r\n";
  if (status == 404)
    header << "Connection: close" << "\r\n";
  else
    header << "Connection: keep-alive" << "\r\n";
  header << "\r\n";
  return (header.str());
}
