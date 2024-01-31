/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 16:01:58 by rnauke            #+#    #+#             */
/*   Updated: 2024/01/31 15:08:25 by rnauke           ###   ########.fr       */
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
	std::vector<size_t> _port;
	std::vector<std::string> _root_dir;
	std::vector<std::string> _server_name;
	std::map<std::string, std::string> _routes;
public:
	Config(std::string path);
	~Config();

	class InvalidConfigurationException : public std::exception
	{
		public:
			virtual const char *what() const throw();
	};

	std::vector<size_t> getPort();
	std::vector<std::string> getName();
	std::vector<std::string> getRoot();
	std::string getIndex(std::string route);
	std::map<std::string, std::string> getRoutes();
	void checkConf(void);
	void parseConf(std::string path);
	void serverDirective(std::ifstream& input);
	void locationDirective(std::ifstream& input);
};

#endif