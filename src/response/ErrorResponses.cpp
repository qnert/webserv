/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorResponses.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 18:33:28 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/23 15:54:28 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void  Server::notImplemented(size_t idx)
{
  this->_clientDetails[idx].setError(true);
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "responseFiles/error501.html");
  std::string length = ft_itos(msg.size());
  std::string response = check_and_add_header(501, this->_data["html"], length, this->_codes, this->_clientDetails[idx]) + msg;
  if (send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0) < 0)
    this->_clientDetails[idx].setConStatus(CLOSE);
}

void  Server::methodNotAllowed(size_t idx)
{
  this->_clientDetails[idx].setError(true);
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "responseFiles/error405.html");
  std::string length = ft_itos(msg.size());
  std::string response = check_and_add_header(405, this->_data["html"], length, this->_codes, this->_clientDetails[idx]) + msg;
  if (send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0) < 0)
    this->_clientDetails[idx].setConStatus(CLOSE);
}

void  Server::NotFound(size_t idx)
{
  this->_clientDetails[idx].setError(true);
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "responseFiles/error404.html");
  std::string length = ft_itos(msg.size());
  std::string response = check_and_add_header(404, this->_data["html"], length, this->_codes, this->_clientDetails[idx]) + msg;
  if (send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0) < 0)
    this->_clientDetails[idx].setConStatus(CLOSE);
}

void  Server::versionNotSupported(size_t idx)
{
  this->_clientDetails[idx].setError(true);
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "responseFiles/error505.html");
  std::string length = ft_itos(msg.size());
  std::string response = check_and_add_header(505, this->_data["html"], length, this->_codes, this->_clientDetails[idx]) + msg;
  if (send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0) < 0)
    this->_clientDetails[idx].setConStatus(CLOSE);
}

void  Server::payloadTooLarge(size_t idx)
{
  this->_clientDetails[idx].setError(true);
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "responseFiles/error413.html");
  std::string length = ft_itos(msg.size());
  std::string response = check_and_add_header(413, this->_data["html"], length, this->_codes, this->_clientDetails[idx]) + msg;
  if (send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0) < 0)
    this->_clientDetails[idx].setConStatus(CLOSE);
}
