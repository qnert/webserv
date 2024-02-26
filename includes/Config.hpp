/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 16:01:58 by rnauke            #+#    #+#             */
/*   Updated: 2024/02/26 10:00:53 by njantsch         ###   ########.fr       */
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
#include "Utils.hpp"

typedef std::vector<std::map<std::string, std::string> > t_confVector;

class Config
{
private:
  std::map<std::string, std::string> _error_pages;
	std::map<std::string, std::string> _config;
	std::vector<std::map<std::string, std::string> > _locations;

	void findOpeningBracket(std::ifstream& input, std::string& line);
	bool findNextServerDirective(std::ifstream& input, std::string& line);
	bool locationExists(std::string uri);
	void checkConf();
	void checkLocation(std::map<std::string, std::string>& map);
public:
	Config(std::ifstream& input);
	~Config();

	std::map<std::string, std::string>& getConfig();
	t_confVector& getLocations();
  std::map<std::string, std::string>& getErrorPages();
	std::map<std::string,std::string> serverDirective(std::ifstream& input);
	std::map<std::string,std::string> locationDirective(std::ifstream& input, std::string& line);
};

template <typename T, typename C>
bool isLessThanMaxValue(T value, C)
{
	return value < std::numeric_limits<C>::max();
}
