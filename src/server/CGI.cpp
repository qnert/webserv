/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:02:44 by skunert           #+#    #+#             */
/*   Updated: 2024/02/26 15:26:32 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CGI_Utils.hpp"
#include "../../includes/CGI.hpp"
#include "../../includes/Server.hpp"

void  CGI::send_error_404(Server& _server){
  std::string header = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n";
  std::string content = storeFileIntoString_cgi(_server.getRightErrorPage("404"));
  header = header + "Content-Length: " + ft_itos(content.length()) + "\r\n\r\n";
  std::string response = header + content;
  if (send(this->_client_fd, response.c_str(), response.size(), 0) <= 0)
    this->_error = 1;
  return ;
}

void  CGI::send_error_405(Server& _server){
  std::string header = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/html\r\n";
  std::string content = storeFileIntoString_cgi(_server.getRightErrorPage("405"));
  header = header + "Content-Length: " + ft_itos(content.length()) + "\r\n\r\n";
  std::string response = header + content;
  if (send(this->_client_fd, response.c_str(), response.size(), 0) <= 0)
    this->_error = 1;
  return ;
}

void  CGI::send_error_500(Server& _server){
  std::string header = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n";
  std::string content = storeFileIntoString_cgi(_server.getRightErrorPage("500"));
  header = header + "Content-Length: " + ft_itos(content.length()) + "\r\n\r\n";
  std::string response = header + content;
  if (send(this->_client_fd, response.c_str(), response.size(), 0) <= 0)
    this->_error = 1;
  return ;
}

void  CGI::send_error_508(Server& _server){
  std::string header = "HTTP/1.1 508 Loop Detected\r\nContent-Type: text/html\r\n";
  std::string content = storeFileIntoString_cgi(_server.getRightErrorPage("508"));
  header = header + "Content-Length: " + ft_itos(content.length()) + "\r\n\r\n";
  std::string response = header + content;
  if (send(this->_client_fd, response.c_str(), response.size(), 0) <= 0)
    this->_error = 1;
  return ;
}

CGI::CGI(int fd, std::string exec_name, std::string body, std::string root, std::string method, Server& _server) : _client_fd(fd), _exec_name(exec_name), _body(body), _root(root), _method(method){
  if (this->_method == "POST")
    this->handle_post(_server);
  else if (this->_method == "GET")
    this->handle_get(_server);
  if (this->_error == 1)
    throw std::runtime_error("Send failed");
}

CGI::~CGI(){}

void  CGI::handle_get(Server& _server){
  this->_error = 0;
  if (this->_exec_name.find("/cgi-bin/") != 0){
    this->send_error_404(_server);
    return ;
  }
  get_path_info_get(this->_exec_name, this->_path_info, this->_body);
  this->_exec_type = get_exec_type(this->_exec_name);
  this->_exec_path = check_exec_type(this->_exec_type);
  this->_exec_name = this->_root + this->_exec_name;
  if (this->_exec_path == "" || this->_exec_name.find("/cgi-bin/") == std::string::npos){
    this->send_error_404(_server);
    return ;
  }
  if (this->_exec_type == ".pl" || this->_exec_type == ".py")
    prepare_execution(_server);
}

void  CGI::handle_post(Server& _server){
  this->_error = 0;
  if (this->_exec_name.find("/cgi-bin/") != 0){
    this->send_error_404(_server);
    return ;
  }
  get_path_info_post(this->_exec_name, this->_path_info);
  this->_exec_type = get_exec_type(this->_exec_name);
  this->_exec_path = check_exec_type(this->_exec_type);
  this->_exec_name = this->_root + this->_exec_name;
  if (this->_exec_path == "" || this->_exec_name.find("/cgi-bin/") == std::string::npos){
    this->send_error_404(_server);
    return ;
  }
  if (this->_exec_type == ".pl" || this->_exec_type == ".py")
    prepare_execution(_server);
}

void   CGI::execute(){
  char *argv[3] = {const_cast<char*>(this->_exec_path.c_str()), const_cast<char*>(this->_exec_name.c_str()), NULL};
  std::string method = "REQUEST_METHOD=" + this->_method;
  std::string length = "CONTENT_LENGTH=" + ft_itos(this->_body.length());
  std::string body = "QUERY_STRING=" + this->_body;
  std::string script_name = "SCRIPT_NAME=" + this->_exec_name;
  std::string path_info = "PATH_INFO=" + this->_path_info;
  char *envp[6] = {const_cast<char*>(method.c_str()), const_cast<char*>(length.c_str()), const_cast<char*>(body.c_str()),
  const_cast<char*>(script_name.c_str()), const_cast<char*>(path_info.c_str()), NULL};
  dup2(this->_client_fd, STDOUT_FILENO);
  execve(argv[0], argv, envp);
  perror("execve fail");
  std::exit(100);
}

void  CGI::prepare_execution(Server& _server)
{
  if (access((this->_exec_name).c_str(), F_OK) == -1){
    this->send_error_404(_server);
    return ;
  }
  else if (access((this->_exec_name).c_str(), X_OK) == -1){
    this->send_error_405(_server);
    return ;
  }
  else{
      std::string msg = add_header_cgi(201, this->_codes);
      if (send(this->_client_fd, msg.c_str(), msg.size(), 0)  <= 0){
        this->_error = 1;
        return ;
      }
  }
  int timeout_seconds = 1;
  int exitcode = 0;
  pid_t fd = fork();
  if (fd == -1)
    return (perror("fork failed"));
  else if (fd == 0){
    this->execute();
    exit(0);
  }
  else{
    sleep(timeout_seconds);
    pid_t result = waitpid(fd, &exitcode, WNOHANG);
    if (result == 0) {
        this->send_error_508(_server);
        kill(fd, SIGKILL);
        waitpid(fd, &exitcode, 0);
    } else if (result < 0) {
        perror("waitpid");
        kill(fd, SIGKILL);
        waitpid(fd, &exitcode, 0);
    }
    else{
      if (WIFEXITED(exitcode)) {
            int exit_code = WEXITSTATUS(exitcode);
            if (exit_code != 0)
              this->send_error_500(_server);
      }
    }
  }
}
