/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 16:01:58 by rnauke            #+#    #+#             */
/*   Updated: 2024/02/21 19:52:38 by rnauke           ###   ########.fr       */
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
	bool _valid;
	
	void findOpeningBracket(std::ifstream& input, std::string& line);
	bool findNextServerDirective(std::ifstream& input, std::string& line);
public:
	Config(std::ifstream& input);
	~Config();

	bool locationExists(const std::string& uri);
	std::map<std::string, std::string>& getConfig();
	t_confVector& getLocations();
	std::map<std::string,std::string> serverDirective(std::ifstream& input);
	std::map<std::string,std::string> locationDirective(std::ifstream& input, std::string& line);
};
