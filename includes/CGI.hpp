/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:59:31 by skunert           #+#    #+#             */
/*   Updated: 2024/02/23 21:47:12 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Statuscodes.hpp"
#include "MIME_type.hpp"
#include <iostream>
#include <unistd.h>
#include <string>
#include <algorithm>

class Statuscodes;
class MIME_type;

class CGI{
  private:
    Statuscodes _codes;
    MIME_type   _data;
    int         _client_fd;
    std::string _exec_type;
    std::string _exec_path;
    std::string _exec_name;
    std::string _path_info;
    std::string _body;
    std::string _root;

  CGI(CGI const& other);
  CGI&  operator=(CGI const& other);

  public:
    void    exec_cgi_default();
    void    send_error_404();
    void    send_error_405();
    void    send_error_500();

  CGI(int fd, std::string exec_name, std::string body, std::string root);
  ~CGI();
};
