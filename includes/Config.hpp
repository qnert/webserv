/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 16:01:58 by rnauke            #+#    #+#             */
/*   Updated: 2024/02/20 18:54:53 by njantsch         ###   ########.fr       */
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
	std::vector<std::map<std::string, std::string> > _configs;
public:
	Config(const std::string& path);
	~Config();

	class InvalidConfigurationException : public std::exception
	{
		public:
			virtual const char *what() const throw();
	};

	t_confVector getConfigs();
	void parseConf(std::string path);
	std::map<std::string,std::string> serverDirective(std::ifstream& input);
	void locationDirective(std::ifstream& input, std::map<std::string, std::string> map);
};
