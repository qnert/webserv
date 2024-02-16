/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 14:35:15 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/16 11:26:13 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

int   Server::getFreeSocket()
{
  for (int i = 1; i < MAX_CLIENTS; i++)
  {
    if (this->_clientPollfds[i].fd == -1)
      return (i);
  }
  return (-1);
}
void  Server::clientsInit()
{
  for (int i = 1; i < MAX_CLIENTS; i++)
  {
    this->_clientPollfds[i].fd = -1;
    this->_clientPollfds[i].events = POLLIN;
    this->_clientPollfds[i].revents = 0;
  }
}

void  Server::cleanUpClientFds()
{
  for (size_t i = 0; i < MAX_CLIENTS; i++) {
    if (this->_clientPollfds[i].fd != -1)
      close(this->_clientPollfds[i].fd);
  }
}

void  Server::removeFd(int i)
{
  std::cout << "Connection closed on idx: " << i << std::endl;
  close(this->_clientPollfds[i].fd);
  this->_clientPollfds[i].fd = -1;
  this->_clientPollfds[i].events = POLLIN;
  this->_clientPollfds[i].revents = 0;
  this->_nfds--;
}

std::string Server::ft_itos(size_t num)
{
  std::ostringstream oss;
  oss << num;
  return (oss.str());
}

MIME_type& Server::getMimeType(void) {return (this->_data);}

Statuscodes& Server::getStatuscodes(void) {return (this->_codes);}
