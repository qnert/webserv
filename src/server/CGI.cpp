/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:02:44 by skunert           #+#    #+#             */
/*   Updated: 2024/02/15 14:29:03 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CGI.hpp"
#include "../../includes/Server.hpp"

static std::string get_exec_type(std::string const& file_path){
  size_t  it;
  it = file_path.find_last_of(".");
  if (it == std::string::npos)
    return (file_path);
  else
    return (file_path.substr(it, file_path.length() - it));
}

static std::string check_exec_type(std::string const& exec_type){
  if (exec_type == ".py"){
    return ("/usr/local/bin/python3");
  }
  else if (exec_type == ".pl"){
    return ("/usr/bin/perl");
  }
  else if (exec_type == ".sh")
    return ("/bin/sh");
  return ("");
}

static std::string  add_header_cgi(int status, Statuscodes& codes)
{
  std::ostringstream header;
  header << "HTTP/1.1 " << status << " " << codes[status];
  return (header.str());
}

static std::string  storeFileIntoString_cgi(std::string path)
{
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

void  get_path_info(std::string&  exec_name, std::string& path_info){
  size_t  start = exec_name.find("responseFiles/cgi-bin/");
  std::string path = exec_name.substr(start + 22, exec_name.length() - start + 22);
  start = path.find_first_of('/');
  if (start == std::string::npos){
    exec_name = exec_name.substr(1, exec_name.length());
    path_info = "/";
    return ;
  }
  else{
    exec_name = "responseFiles/cgi-bin/" + path.substr(0, start);
    path_info = path.substr(start, path_info.length() - start);
    return ;
  }
}

void  CGI::send_error_405(void){
  std::string header = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/html\r\n";
  std::string content = storeFileIntoString_cgi("./responseFiles/error405.html");
  header = header + "Content-Length: " + Server::ft_itos(content.length()) + "\r\n\r\n";
  std::string response = header + content;
  send(this->_client_fd, response.c_str(), response.size(), 0);
  return ;
}

void  CGI::send_error_500(){
  std::string header = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n";
  std::string content = storeFileIntoString_cgi("./responseFiles/error500.html");
  header = header + "Content-Length: " + Server::ft_itos(content.length()) + "\r\n\r\n";
  std::string response = header + content;
  send(this->_client_fd, response.c_str(), response.size(), 0);
  return ;
}

CGI::CGI(int fd, std::string exec_name, std::string body) : _client_fd(fd), _exec_name(exec_name), _body(body){
  get_path_info(this->_exec_name, this->_path_info);
  this->_exec_type = get_exec_type(this->_exec_name);
  this->_exec_path = check_exec_type(this->_exec_type);
  if (this->_exec_path == "" || this->_exec_name.find("responseFiles/cgi-bin/") == std::string::npos){
    std::cout << "first_exit\n";
    this->send_error_405();
    return ;
  }
  if (this->_exec_type == ".pl" || this->_exec_type == ".py" || this->_exec_type == ".sh")
    exec_cgi_default();
}

CGI::~CGI(){}

void  CGI::exec_cgi_default(){
  if (access((this->_exec_name).c_str(), F_OK) == -1 || access((this->_exec_name).c_str(), X_OK) == -1){
      this->send_error_405();
      return ;
  }
  else{
      std::string msg = add_header_cgi(201, _codes);
      send(this->_client_fd, msg.c_str(), msg.size(), 0);
  }
  int fd = fork();
  if (fd == 0){
    char *argv[3] = {const_cast<char*>(this->_exec_path.c_str()), const_cast<char*>(this->_exec_name.c_str()), NULL};
    std::string length = "CONTENT_LENGTH=" + Server::ft_itos(this->_body.length());
    std::string body = "QUERY_STRING=" + this->_body;
    std::string script_name = "SCRIPT_NAME=" + this->_exec_name;
    std::string path_info = "PATH_INFO=" + this->_path_info;
    char *envp[6] = {const_cast<char*>("REQUEST_METHOD=POST"), const_cast<char*>(length.c_str()), const_cast<char*>(body.c_str()),
      const_cast<char*>(script_name.c_str()), const_cast<char*>(path_info.c_str()), NULL};
    dup2(this->_client_fd, STDOUT_FILENO);
    execve(argv[0], argv, envp);
    this->send_error_500();
    std::exit(0);
  }
}

