/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:59:31 by skunert           #+#    #+#             */
/*   Updated: 2024/02/15 11:07:10 by skunert          ###   ########.fr       */
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

  CGI(CGI const& other);
  CGI&  operator=(CGI const& other);

  public:
    void    exec_cgi_default();
    void    send_error_405();
    void    send_error_500();

  CGI(int fd, std::string exec_name, std::string body);
  ~CGI();
};
