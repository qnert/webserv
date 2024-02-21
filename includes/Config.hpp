/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 16:01:58 by rnauke            #+#    #+#             */
/*   Updated: 2024/02/21 04:32:17 by rnauke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <unistd.h>

typedef std::vector<std::map<std::string, std::string> > t_confVector;

class Config
{
private:
	std::map<std::string, std::string> _config;
	std::vector<std::map<std::string, std::string> > _locations;
public:
	Config(const std::string& path);
	~Config();

	std::map<std::string, std::string> getConfig();
	t_confVector getLocations();
	void parseConf(std::string path);
	std::map<std::string,std::string> serverDirective(std::ifstream& input);
	std::map<std::string,std::string> locationDirective(std::ifstream& input, std::string& line);
};
