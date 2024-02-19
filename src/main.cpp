/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:35 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/19 04:32:15 by rnauke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/MIME_type.hpp"
#include "../includes/Statuscodes.hpp"
#include "../includes/Config.hpp"

void cleanUpClientFds(struct pollfd* pollfds)
{
  for (size_t i = 0; i < MAX_CLIENTS; i++)
    close(pollfds[i].fd);
}

Server* isServerSocket(int fd, std::vector<Server> servers)
{
	for (std::vector<Server>::iterator i = servers.begin(); i < servers.end(); ++i)
	{
		if (fd == i.base()->getFd())
			return &(*i);
	}
	return (Server*)NULL;
}


void removeFd(struct pollfd* pollfds)
{
  close(pollfds->fd);
  pollfds->fd = -1;
  pollfds->events = POLLIN;
  pollfds->revents = 0;
}

void handleRequest(struct pollfd* pollfd, Clients* client)
{
  while (true)
  {
    char buffer[10000];
    ssize_t bytesRead = recv(pollfd->fd, buffer, sizeof(buffer) - 1, O_NONBLOCK);

    if (bytesRead < 0) {
      break;
    }
    if (bytesRead == 0 && client->getPendingReceive() == false) {
      std::cout << "Client has closed the connection" << std::endl;
      removeFd(pollfd);
      break;
    }
    buffer[bytesRead] = '\0';
    client->parseRequestBuffer(buffer, bytesRead);
    if (client->getPendingReceive() == false)
      pollfd->events = POLLOUT;
  }
}

Server* matchRequestToServer(std::vector<Server> servers, Clients* client)
{
	// implement that if theres no server name provided any connection with the correct port gets accepted
	std::string hostname = client->getMapValue("Host");
	for (std::vector<Server>::iterator i = servers.begin(); i < servers.end(); ++i)
	{
		if (i.base()->getServername() == hostname.substr(0, hostname.find(':'))
			&& i.base()->getPort() == hostname.substr(hostname.find(':')))
			return &(*i);
	}
	return NULL;
	
}

void serverLoop(std::vector<Server> servers, struct pollfd* pollfds, Clients* clientDetails)
{
	Server* currentServer;
	
	while (true)
	{
		if (poll(pollfds, MAX_CLIENTS, 10000) < 0)
		{
			perror("poll");
			// there needs to be a function that closes all open fds in case of a crash
			// close(this->_serverSocket);
			throw(std::runtime_error(""));
		}
		for (size_t i = 0; i < MAX_CLIENTS; i++)
		{
			if (pollfds[i].revents == 0)
			continue;
	
			// error check revents
			if (pollfds[i].revents & POLLHUP)
				removeFd(&(pollfds[i]));
			else if ((pollfds[i].revents & POLLERR) || (pollfds[i].revents & POLLNVAL))
			{
				cleanUpClientFds(pollfds);
				throw(std::runtime_error("poll_revents error"));
			}


			if (pollfds[i].revents == POLLIN)
			{
				if ((currentServer = isServerSocket(pollfds[i].fd, servers)))
					currentServer->acceptConnections();
				else
					handleRequest(&(pollfds[i]), &(clientDetails[i]));
				// put handlerequest in main and figure out which server it belongs to by comparing "Host:"
				// then just return the server it belongs to and send answer or cgi or anything :)
			}
			currentServer = matchRequestToServer(servers, &(clientDetails[i]));
			if (pollfds[i].revents == POLLOUT)
			{
				currentServer->sendAnswer(i);
				clientDetails[i].cleanUp();
			}
		} // * END OF CLIENT LOOP *
	}
}

int main(int argc, char** argv)
{
	std::vector<Server> servers;
	struct pollfd       pollfds[MAX_CLIENTS];
	Clients             clientDetails[MAX_CLIENTS];
  try
  {
    MIME_type     data_types;
    Statuscodes   statuscodes;
	if (argc != 2)
		return 1;
	Config cfg = Config(argv[1]);
	std::vector<std::map<std::string, std::string> > con = cfg.getConfigs();
	for (std::vector<std::map<std::string, std::string> >::iterator i = con.begin(); i < con.end(); ++i)
	{
    	servers.push_back(Server(data_types, statuscodes, pollfds, clientDetails, *i.base()));
	}
  	serverLoop(servers, pollfds, clientDetails);
    cleanUpClientFds(pollfds);
}
  catch(const std::exception& e)
  {
    std::cout << e.what() << '\n';
  }
  return (0);
}
