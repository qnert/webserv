/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Locations.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 15:37:26 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/23 18:53:07 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void  Server::initConfVars(Config& cfg)
{
  std::map<std::string, std::string> config = cfg.getConfig();

  std::cout << "server port: " << ft_stosh(config.find("listen")->second) << std::endl;
  this->_port = config.find("listen")->second;
  this->_servername = config.find("server_name")->second;
  this->_root = config.find("root")->second;
  if (config.find("max_client_body") != config.end())
    this->_maxClientBody = ft_stosize(config.find("max_client_body")->second);
  else
    this->_maxClientBody = LONG_MAX;
  this->_locations = cfg.getLocations();
}

bool  Server::checkLocationPrelims(std::string method)
{
  if (!this->_currLocation.empty())
  {
    if (this->_currLocation["deny_methods"].find(method) != std::string::npos)
      return (true);
  }
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

