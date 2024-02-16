/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:35 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/16 04:18:32 by rnauke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/MIME_type.hpp"
#include "../includes/Statuscodes.hpp"
#include "../includes/Config.hpp"


int main(int argc, char** argv)
{
	std::vector<Server> servers;
	struct pollfd       pollfds[MAX_CLIENTS];
	Clients             clientDetails[MAX_CLIENTS];
  try
  {
    MIME_type     data_types;
    Statuscodes   statuscodes;
	  if (argc == 2)
	    Config cfg = Config(argv[1]);
	for (std::vector<std::map<std::string, std::string> > i = cfg.getConfigs().begin(); i < cfg.getConfigs().end(); i++)
	{
    	servers.push_back(Server(data_types, statuscodes, pollfds, clientDetails, *i));
	}
	
	if (poll(this->_clientPollfds, MAX_CLIENTS, 10000) < 0)
	{
      perror("poll");
      close(this->_serverSocket);
      throw(std::runtime_error(""));
    }
    httpServer.serverLoop();
	
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << '\n';
  }
  return (0);
}
