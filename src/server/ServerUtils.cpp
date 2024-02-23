/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 14:35:15 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/23 12:49:45 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

int   Server::getFreeSocket()
{
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    if (this->_clientPollfds[i].fd == -1
        && this->_clientDetails[i].getFdStatus() == UNUSED)
      return (i);
	}
  return (-1);
}

void  Server::cleanUpClientFds()
{
  for (size_t i = 0; i < MAX_CLIENTS; i++) {
    if (this->_clientDetails[i].getFdStatus() != UNUSED)
      close(this->_clientPollfds[i].fd);
  }
}

void  Server::removeFd(int i)
{
  if (close(this->_clientPollfds[i].fd) == -1)
    perror("removeFd: close");
  this->_clientDetails[i].cleanUp();
  this->_clientDetails[i].cleanUpResponse();
  this->_clientDetails[i].setFdStatus(UNUSED);
  this->_clientDetails[i].setConStatus(KEEPALIVE);
  this->_clientPollfds[i].fd = -1;
  this->_clientPollfds[i].events = POLLIN;
  this->_clientPollfds[i].revents = 0;
  this->_nfds--;
  std::cout << "Connection closed at idx: " << i << std::endl;
}

std::string Server::ft_itos(size_t num)
{
  std::ostringstream oss;
  oss << num;
  return (oss.str());
}

bool  Server::checkLocationPrelims(std::string method, size_t idx)
{
  if (!this->_currLocation.empty())
  {
    if (this->_currLocation["deny_methods"].find(method) != std::string::npos)
      return (true);
  }
  else if (checkReferer(method, idx) == true)
    return (true);
  return (false);
}

void  Server::getCurrLocation(size_t index)
{
  std::vector<t_strmap>::iterator it;
  for (it = this->_locations.begin(); it != this->_locations.end(); ++it)
  {
    if (this->_clientDetails[index].getUri() == it->find("uri")->second) {
      this->_currLocation = *it;
      break ;
    }
  }
}

bool  Server::checkReferer(std::string method, size_t index)
{
  std::vector<t_strmap>::iterator it;
  std::string referer = this->_clientDetails[index].getMapValue("Referer");
  if (!referer.empty()) {
    size_t pos = referer.find_last_of("/");
    std::string refererUri = referer.substr(pos);
    for (it = this->_locations.begin(); it != this->_locations.end(); ++it)
    {
      if (refererUri != it->find("uri")->second)
        continue;
      if (it->find("deny_methods") == it->end())
        continue;
      if (it->find("deny_methods")->second.find(method) != std::string::npos)
        return (true);
      else
        return (false);
    }
  }
  return (false);
}

MIME_type& Server::getMimeType(void) {return (this->_data);}

Statuscodes& Server::getStatuscodes(void) {return (this->_codes);}
