/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:35:23 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/21 14:39:47 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ServerManager.hpp"

ServerManager::ServerManager(Config config)
{
  MIME_type     data_types;
  Statuscodes   statuscodes;
  t_confVector  configs = config.getConfigs();
  this->clientsInit();
	for (t_confVector::iterator i = configs.begin(); i < configs.end(); ++i)
	{
    	this->_servers.push_back(Server(data_types, statuscodes, this->_clientPollfds, this->_clientDetails, *i.base()));
	}
  this->serverLoop();
  this->cleanUpClientFds();
}

ServerManager::~ServerManager() {}

bool  ServerManager::checkRevents(size_t index)
{
  int error = 0;

  if (this->_clientPollfds[index].revents & POLLHUP)
    error = 1;
  else if (this->_clientPollfds[index].revents & POLLERR)
    error = 1;
  else if (this->_clientPollfds[index].revents & POLLNVAL)
    error = 1;
  else if (this->_clientPollfds[index].revents & POLLPRI)
    error = 1;
  if (error == 1)
  {
    this->_currentServer.removeFd(index);
    return (true);
  }
  return (false);
}

void ServerManager::handleRequest(size_t i)
{
  char buffer[10000];
  ssize_t bytesRead = recv(this->_clientPollfds[i].fd, buffer, sizeof(buffer) - 1, O_NONBLOCK);

  if (bytesRead < 0) {
    perror("recv");
    this->_currentServer.removeFd(i);
    return;
  }
  if (bytesRead == 0 && this->_clientDetails[i].getPendingReceive() == false) {
    std::cout << "Client has closed the connection" << std::endl;
    this->_currentServer.removeFd(i);
    return;
  }
  buffer[bytesRead] = '\0';
  this->_clientDetails[i].parseRequestBuffer(buffer, bytesRead);
  if (this->_clientDetails[i].getPendingReceive() == false)
    this->_clientPollfds[i].events = POLLOUT;
}

void ServerManager::serverLoop()
{
	while (true)
	{
		if (poll(this->_clientPollfds, MAX_CLIENTS, 5000) < 0)
		{
			perror("poll");
			// there needs to be a function that closes all open fds in case of a crash
			// close(this->_serverSocket);
			throw(std::runtime_error(""));
		}
		for (size_t i = 0; i < MAX_CLIENTS; i++)
		{
			if (this->_clientPollfds[i].revents == 0 || checkRevents(i) == true)
			  continue;

			if (this->_clientPollfds[i].revents == POLLIN)
			{
				if (isServerSocket(this->_clientPollfds[i].fd))
					this->_currentServer.acceptConnections();
				else
				{
					this->handleRequest(i);
					matchRequestToServer(i);
				}
			}
			else if (this->_clientPollfds[i].revents == POLLOUT)
			{
				this->_currentServer.sendAnswer(i);
			}
		} // * END OF CLIENT LOOP *
	}
}
