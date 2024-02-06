/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:00:07 by skunert           #+#    #+#             */
/*   Updated: 2024/02/05 17:04:08 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/RequestUtils.hpp"

std::string  storeFileIntoString(RequestParser req, std::string path)
{
  if (req.getUri() == "/" && path != "responseFiles/error501.html")
    path = req.getCurrdir() + "responseFiles/index.html";
  else
    path = req.getCurrdir() + path;
  std::ifstream file(path, std::ios::binary);
  if (!file.is_open())
    return ("");

  std::string fileContent;
  std::ostringstream buffer;
  buffer << file.rdbuf();
  fileContent = buffer.str();
  file.close();
  return (fileContent);
}

std::string get_first_name(std::string body){
  size_t  start_pos = body.find_first_of("=") + 1;
  size_t  end_pos = body.find_first_of("&") - body.find_first_of("=") - 1;
  std::string ret_str = body.substr(start_pos, end_pos);

  return (ret_str);
}

std::string get_last_name(std::string body){
  size_t  start_pos = body.find_last_of("=") + 1;
  size_t  end_pos = body.find_last_of("&") - body.find_last_of("=") - 1;
  std::string ret_str = body.substr(start_pos, end_pos);

  return (ret_str);
}

std::string get_filecontent(std::string body){
  size_t start = body.find("\n\n") + 2;
  size_t end = body.find("\n------");
  std::string filecontent = body.substr(start, end - start);

  return (filecontent);
}

std::string get_filename(std::string body){
  std::string filename = body.substr(body.find("filename=") + 10);
  filename = filename.substr(0, filename.find('\n') - 2);
  return (filename);
}

void  handle_name_input(int fd, RequestParser req){
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

void  handle_file_upload(int fd, RequestParser req){
  char *argv[5];
  std::string cgi_filename = "./cpp_uploadfile.cgi";
  std::string file_fd = Server::ft_itos(fd);
  std::string filename = get_filename(req.getBody());
  std::string filecontent = get_filecontent(req. getBody());
  argv[0] = const_cast<char*>(cgi_filename.c_str());
  argv[1] = const_cast<char*>(file_fd.c_str());
  argv[2] = const_cast<char*>(filename.c_str());
  argv[3] = const_cast<char*>(filecontent.c_str());
  argv[4] = NULL;
  execve("/Users/skunert/Documents/webserv/responseFiles/cpp_uploadfile.cgi", argv, NULL);
}

void  handle_Request_post(int fd, RequestParser req){
  if (req.getUri() == "/responseFiles/first.cgi")
    handle_name_input(fd, req);
  else if (req.getUri() == "upload")
    handle_file_upload(fd, req);
}

std::string  check_and_add_header(int status, std::string const& type, MIME_type data, Statuscodes codes){
  std::ostringstream header;
  header << "HTTP/1.1 " << status << " " << codes[status] << "\r\n";
  header << "Content-Type: " << data[type] << "\r\n";
  header << "Connection: keep-alive" << "\r\n";
  header << "\r\n";
  return (header.str());
}
