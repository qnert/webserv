/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Locations.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 15:37:26 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/25 20:04:51 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void  Server::initConfVars(Config& cfg)
{
  std::map<std::string, std::string> config = cfg.getConfig();

  std::cout << "server port: " << ft_stosh(config.find("listen")->second) << std::endl;
  this->_port = config.find("listen")->second;
  this->_servername = config.find("server_name")->second;
  this->_index = config.find("index")->second;
  this->_serverRoot = config.find("root")->second;
  this->_maxClientBody = ft_stosize(config.find("max_client_body")->second);
  this->_locations = cfg.getLocations();
}

bool  Server::checkLocationPrelims(std::string method)
{
  if (!this->_currLocation.empty())
  {
    if (this->_currLocation["deny_methods"].find(method) != std::string::npos
        || !this->_currLocation["redirect"].empty())
      return (true);
  }
  return (false);
}

void  Server::getCurrLocation(size_t index)
{
  std::vector<t_strmap>::iterator it;
  std::string uri = this->_clientDetails[index].getUri();
  for (it = this->_locations.begin(); it != this->_locations.end(); ++it)
  {
    if (uri == it->find("uri")->second) {
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
    this->_clientDetails[idx].setCurrDir(this->_serverRoot);
}

std::string Server::getRightIndexFile()
{
	std::string token, path, stream;

	if (!this->_currLocation.empty())
    stream = this->_currLocation["index"];
	else
		stream = _index;
	std::istringstream str(stream);
	while (str >> token)
	{
		if (!this->_currLocation.empty())
    		path = this->_currLocation["root"] + "/" + token;
		else
			path = this->_serverRoot + "/" + token;
		std::ifstream file(path);
    	if (file.good())
			return "/" + token;
	}
	return "";
}
