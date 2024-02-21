/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestUtils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:00:34 by skunert           #+#    #+#             */
/*   Updated: 2024/02/21 11:12:05 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <dirent.h>
#include "Statuscodes.hpp"
#include "MIME_type.hpp"
#include "Server.hpp"

std::string  storeFileIntoString(Clients& req, std::string path);

std::string get_first_name(std::string body);

std::string get_last_name(std::string body);

std::string get_filecontent(std::string boundary, std::string body);

std::string get_filename(std::string body);

void  handle_name_input(int fd, Clients& req);

void  handle_file_upload(int fd, Clients& req, MIME_type& data, Statuscodes& codes);

std::string  handle_file_erasing(int fd, Clients& req, Statuscodes& codes);

void  list_directories(int fd, Clients& req, Statuscodes& codes, DIR* dir);

std::string  check_and_add_header(int status, std::string const& type, std::string const& length, \
                                  Statuscodes& codes, Clients& req);
