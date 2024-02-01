/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestUtils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: simonkunert <simonkunert@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:00:34 by skunert           #+#    #+#             */
/*   Updated: 2024/02/01 20:43:55 by simonkunert      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "Statuscodes.hpp"
#include "MIME_type.hpp"
#include "RequestParser.hpp"

std::string  storeFileIntoString(RequestParser req, std::string path);

std::string get_first_name(std::string body);

std::string get_last_name(std::string body);

std::string get_filecontent(std::string body);

std::string get_filename(std::string body);

void  handle_name_input(int fd, RequestParser req);

void  handle_file_upload(int fd, RequestParser req, MIME_type& data, Statuscodes& codes);

void  handle_file_erasing(int fd, RequestParser req, MIME_type& data, Statuscodes& codes);

void  handle_Request_post(int fd, RequestParser req, MIME_type& data, Statuscodes& codes);

std::string  check_and_add_header(int status, std::string const& type, MIME_type data, Statuscodes codes);
