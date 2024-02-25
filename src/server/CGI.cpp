/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:02:44 by skunert           #+#    #+#             */
/*   Updated: 2024/02/25 21:01:38 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CGI.hpp"
#include "../../includes/Server.hpp"

static std::string get_exec_type(std::string const& file_path){
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

void  get_path_info_get(std::string&  exec_name, std::string& path_info){
  size_t  start = exec_name.find("/cgi-bin/");
  if (start == std::string::npos){
    exec_name = "";
    return ;
  }
  size_t  end = exec_name.find_first_of('?', start);
  if (exec_name.find('?', end + 1) != std::string::npos){
    exec_name = "";
    return ;
  }
  path_info = exec_name.substr(exec_name.find_last_of('?') + 1, exec_name.length());
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
    path_info = path.substr(start + 1, path_info.length() - start + 1);
    return ;
  }
}

void  CGI::send_error_404(void){
  std::string header = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n";
  std::string content = storeFileIntoString_cgi("./responseFiles/error404.html");
  header = header + "Content-Length: " + ft_itos(content.length()) + "\r\n\r\n";
  std::string response = header + content;
  if (send(this->_client_fd, response.c_str(), response.size(), 0) <= 0)
    this->_error = 1;
  return ;
}

void  CGI::send_error_405(void){
  std::string header = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/html\r\n";
  std::string content = storeFileIntoString_cgi("./responseFiles/error405.html");
  header = header + "Content-Length: " + ft_itos(content.length()) + "\r\n\r\n";
  std::string response = header + content;
  if (send(this->_client_fd, response.c_str(), response.size(), 0) <= 0)
    this->_error = 1;
  return ;
}

void  CGI::send_error_500(){
  std::string header = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n";
  std::string content = storeFileIntoString_cgi("./responseFiles/error500.html");
  header = header + "Content-Length: " + ft_itos(content.length()) + "\r\n\r\n";
  std::string response = header + content;
  if (send(this->_client_fd, response.c_str(), response.size(), 0) <= 0)
    this->_error = 1;
  return ;
}

void  CGI::send_error_508(){
  std::string header = "HTTP/1.1 508 Loop Detected\r\nContent-Type: text/html\r\n";
  std::string content = storeFileIntoString_cgi("./responseFiles/error508.html");
  header = header + "Content-Length: " + ft_itos(content.length()) + "\r\n\r\n";
  std::string response = header + content;
  if (send(this->_client_fd, response.c_str(), response.size(), 0) <= 0)
    this->_error = 1;
  return ;
}

CGI::CGI(int fd, std::string exec_name, std::string body, std::string root, std::string method) : _client_fd(fd), _exec_name(exec_name), _body(body), _root(root), _method(method){
  if (this->_method == "POST")
    this->handle_post();
  else if (this->_method == "GET")
    this->handle_get();
}

CGI::~CGI(){}

void  CGI::handle_get(){
  this->_error = 0;
  if (this->_exec_name.find("/cgi-bin/") != 0){
    this->send_error_404();
    return ;
  }
  get_path_info_get(this->_exec_name, this->_path_info);
  this->_exec_type = get_exec_type(this->_exec_name);
  this->_exec_path = check_exec_type(this->_exec_type);
  this->_exec_name = this->_root + this->_exec_name;
  if (this->_exec_path == "" || this->_exec_name.find("/cgi-bin/") == std::string::npos){
    this->send_error_404();
    return ;
  }
  if (this->_exec_type == ".pl" || this->_exec_type == ".py" || this->_exec_type == ".sh")
    prepare_execution();
}

void  CGI::handle_post(){
  this->_error = 0;
  if (this->_exec_name.find("/cgi-bin/") != 0){
    this->send_error_404();
    return ;
  }
  get_path_info_post(this->_exec_name, this->_path_info);
  this->_exec_type = get_exec_type(this->_exec_name);
  this->_exec_path = check_exec_type(this->_exec_type);
  this->_exec_name = this->_root + this->_exec_name;
  if (this->_exec_path == "" || this->_exec_name.find("/cgi-bin/") == std::string::npos){
    this->send_error_404();
    return ;
  }
  if (this->_exec_type == ".pl" || this->_exec_type == ".py" || this->_exec_type == ".sh")
    prepare_execution();
}

void   CGI::execute(){
  char *argv[3] = {const_cast<char*>(this->_exec_path.c_str()), const_cast<char*>(this->_exec_name.c_str()), NULL};
  std::string method = "REQUEST_METHOD=" + this->_method;
  std::string length = "CONTENT_LENGTH=" + ft_itos(this->_body.length());
  std::string body;
  if (this->_method == "GET")
    body = "QUERY_STRING=" + this->_path_info;
  else
    body = "QUERY_STRING=" + this->_body;
  std::string script_name = "SCRIPT_NAME=" + this->_exec_name;
  std::string path_info;
  if (this->_method == "GET")
    path_info = "PATH_INFO=";
  else
    path_info = "PATH_INFO=" + this->_path_info;
  char *envp[6] = {const_cast<char*>(method.c_str()), const_cast<char*>(length.c_str()), const_cast<char*>(body.c_str()),
  const_cast<char*>(script_name.c_str()), const_cast<char*>(path_info.c_str()), NULL};
  dup2(this->_client_fd, STDOUT_FILENO);
  execve(argv[0], argv, envp);
  perror("execve fail");
  std::exit(100);
}

void  CGI::prepare_execution()
{
  if (access((this->_exec_name).c_str(), F_OK) == -1){
    this->send_error_404();
    return ;
  }
  else if (access((this->_exec_name).c_str(), X_OK) == -1){
    this->send_error_405();
    return ;
  }
  else{
      std::string msg = add_header_cgi(201, this->_codes);
      if (send(this->_client_fd, msg.c_str(), msg.size(), 0)  <= 0){
        this->_error = 1;
        return ;
      }
  }
  int timeout_seconds = 3;
  int exitcode = 0;
  pid_t fd = fork();
  if (fd == -1)
    return (perror("fork failed"));
  else if (fd == 0){
    this->execute();
  }
  else{
    sleep(timeout_seconds);
    pid_t result = waitpid(fd, &exitcode, WNOHANG);
    if (result == 0) {
        this->send_error_508();
        kill(fd, SIGKILL);
        waitpid(fd, &exitcode, 0);
    } else if (result < 0) {
        perror("waitpid");
        kill(fd, SIGKILL);
        waitpid(fd, &exitcode, 0);
    }
    else{
      if (WIFEXITED(exitcode))
        this->send_error_500();
    }
  }
}
