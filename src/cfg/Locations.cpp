/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Locations.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 15:37:26 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/23 16:29:34 by njantsch         ###   ########.fr       */
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
