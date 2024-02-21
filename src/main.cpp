/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:35 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/21 04:40:04 by rnauke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerManager.hpp"
#include "../includes/MIME_type.hpp"
#include "../includes/Statuscodes.hpp"
#include "../includes/Config.hpp"

int main(int argc, char** argv)
{
  if (argc > 2)
    return 1;
  try
  {
    std::string path;
	if (argc < 2)
		path = "config/default.conf";
    else
      path = argv[1];
    Config cfg(path);
	std::map<std::string, std::string> servercon = cfg.getConfig();
	for (std::map<std::string, std::string>::iterator i = servercon.begin(); i != servercon.end(); ++i)
		std::cout << "server: " << i->first << "->" << i->second << std::endl;
	std::vector<std::map<std::string, std::string> > locations = cfg.getLocations();
	for (std::vector<std::map<std::string, std::string> >::iterator i = locations.begin(); i != locations.end(); ++i)
		for (std::map<std::string, std::string>::iterator j = i.base()->begin(); j != i.base()->end(); ++j)
			std::cout << "location: " << j->first << "->" << j->second << std::endl;
    // ServerManager servers(cfg);
    // servers.serverLoop();
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << '\n';
  }
  return (0);
}
