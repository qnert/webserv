/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorResponses.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 18:33:28 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/24 13:35:21 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void  Server::handleRedirectLocation(size_t idx)
{
  this->_clientDetails[idx].setRedirect(this->_currLocation["redirect"]);
  std::string msg = "Moved Temporarily\n";
  std::string length = ft_itos(msg.size());
  std::string response = check_and_add_header(302, this->_data["plain"], length, \
                                            this->_codes, this->_clientDetails[idx]) + msg;
  send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0);
  this->_clientDetails[idx].setConStatus(CLOSE);
}

void  Server::notImplemented(size_t idx)
{
  this->_clientDetails[idx].setError(true);
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "error501.html");
  std::string length = ft_itos(msg.size());
  std::string response = check_and_add_header(501, this->_data["html"], length, this->_codes, this->_clientDetails[idx]) + msg;
  if (send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0) < 0)
    this->_clientDetails[idx].setConStatus(CLOSE);
}

void  Server::methodNotAllowed(size_t idx)
{
  this->_clientDetails[idx].setError(true);
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "error405.html");
  std::string length = ft_itos(msg.size());
  std::string response = check_and_add_header(405, this->_data["html"], length, this->_codes, this->_clientDetails[idx]) + msg;
  if (send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0) < 0)
    this->_clientDetails[idx].setConStatus(CLOSE);
}

void  Server::NotFound(size_t idx)
{
  this->_clientDetails[idx].setError(true);
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "error404.html");
  std::string length = ft_itos(msg.size());
  std::string response = check_and_add_header(404, this->_data["html"], length, this->_codes, this->_clientDetails[idx]) + msg;
  if (send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0) < 0)
    this->_clientDetails[idx].setConStatus(CLOSE);
}

void  Server::versionNotSupported(size_t idx)
{
  this->_clientDetails[idx].setError(true);
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "error505.html");
  std::string length = ft_itos(msg.size());
  std::string response = check_and_add_header(505, this->_data["html"], length, this->_codes, this->_clientDetails[idx]) + msg;
  if (send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0) < 0)
    this->_clientDetails[idx].setConStatus(CLOSE);
}

void  Server::payloadTooLarge(size_t idx)
{
  this->_clientDetails[idx].setError(true);
  std::string msg = storeFileIntoString(this->_clientDetails[idx], "error413.html");
  std::string length = ft_itos(msg.size());
  std::string response = check_and_add_header(413, this->_data["html"], length, this->_codes, this->_clientDetails[idx]) + msg;
  if (send(this->_clientPollfds[idx].fd, response.c_str(), response.size(), 0) < 0)
    this->_clientDetails[idx].setConStatus(CLOSE);
}
