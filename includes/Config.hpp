/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 16:01:58 by rnauke            #+#    #+#             */
/*   Updated: 2024/02/19 03:42:42 by rnauke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <unistd.h>

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

	std::vector<std::map<std::string, std::string> > getConfigs();
	void parseConf(std::string path);
	std::map<std::string,std::string> serverDirective(std::ifstream& input);
	void locationDirective(std::ifstream& input, std::map<std::string, std::string> map);
};

#endif