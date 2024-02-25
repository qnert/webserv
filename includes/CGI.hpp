/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:59:31 by skunert           #+#    #+#             */
/*   Updated: 2024/02/25 18:56:14 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Statuscodes.hpp"
#include "MIME_type.hpp"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <algorithm>

class Statuscodes;
class MIME_type;

class CGI{
  private:
    Statuscodes _codes;
    MIME_type   _data;
    int         _client_fd;
    int         _error;
    std::string _exec_type;
    std::string _exec_path;
    std::string _exec_name;
    std::string _path_info;
    std::string _body;
    std::string _root;
    std::string _server_name;

  CGI(CGI const& other);
  CGI&  operator=(CGI const& other);

  public:
    void    execute();
    void    prepare_execution();
    void    send_error_404();
    void    send_error_405();
    void    send_error_500();
    void    send_error_508();

  CGI(int fd, std::string exec_name, std::string body, std::string root);
  ~CGI();
};
