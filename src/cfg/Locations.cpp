/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Locations.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 15:37:26 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/24 14:35:15 by rnauke           ###   ########.fr       */
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
  this->_index = config.find("index")->second;
  this->_maxClientBody = ft_stosize(config.find("max_client_body")->second);
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

void  Server::setRightCurrDir(size_t idx)
{
  if (!this->_currLocation.empty()) {
    this->_clientDetails[idx].setCurrDir(this->_currLocation["root"]);
  }
  else
    this->_clientDetails[idx].setCurrDir(this->_root);
}

std::string Server::getIndexFile()
{
	std::string token, path, stream;
	
	if (!this->_currLocation.empty())
	{
		std::cout << "found: " << this->_currLocation["index"] << std::endl;
    	stream = this->_currLocation["index"];
	}
	else
	{
		stream = _index;
		std::cout << "inside: " << _index << std::endl;
	}
	std::istringstream str(stream);
	while (str >> token)
	{
		if (!this->_currLocation.empty())
		{
    		path = this->_currLocation["root"] + "/" + token;
			std::cout << "full path: " << this->_currLocation["root"] + "/" + token << std::endl;
		}
		else
		{
			path = this->_root + "/" + token;
			std::cout << "full path: " << _root + "/" + token << std::endl;
		}
		std::ifstream file(path);
    	if (file.good())
			return token;
	}
	return "";
}
