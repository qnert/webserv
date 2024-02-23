/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManagerUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:36:32 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/23 16:54:26 by rnauke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ServerManager.hpp"

void ServerManager::cleanUpClientFds()
{
  for (size_t i = 0; i < MAX_CLIENTS; i++) {
    if (this->_clientDetails[i].getFdStatus() == USED)
      close(this->_clientPollfds[i].fd);
  }
}

void ServerManager::clientsInit()
{
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    this->_clientPollfds[i].fd = -1;
    this->_clientPollfds[i].events = POLLIN;
    this->_clientPollfds[i].revents = 0;
  }
}

bool ServerManager::isServerSocket(int fd)
{
	for (std::vector<Server>::iterator i = this->_servers.begin(); i < this->_servers.end(); ++i)
	{
		if (fd == (*i).getFd()) {
			this->_currentServer = *i;
      return (true);
    }
	}
	return (false);
}

void ServerManager::matchRequestToServer(size_t index)
{
	// implement that if theres no server name provided any connection with the correct port gets accepted
	std::string hostname =  this->_clientDetails[index].getMapValue("Host");
	if (hostname.find(':') == std::string::npos)
		hostname.append(":80");
	for (std::vector<Server>::iterator i = this->_servers.begin(); i < this->_servers.end(); ++i)
	{
		if ((*i).getServername() == hostname.substr(0, hostname.find(':')) && (*i).getPort() == hostname.substr(hostname.find(':')+1))
		{
			this->_currentServer = *i;
			_clientDetails[index].setConStatus(KEEPALIVE);
			return;
		}
		else if ((*i).getPort() == hostname.substr(hostname.find(':') + 1))
		{
			this->_currentServer = *(_servers.begin());
			_clientDetails[index].setConStatus(KEEPALIVE);
			return;
		}
		else
			_clientDetails[index].setConStatus(CLOSE);
	}
}
