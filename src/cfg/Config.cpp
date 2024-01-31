/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 16:11:27 by rnauke            #+#    #+#             */
/*   Updated: 2024/01/31 15:28:30 by rnauke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Config.hpp"

static int ft_stoi(const std::string& str)
{
	int num;
	std::stringstream ss(str);

	ss >> num;
	return num;
}

std::string trim(const std::string& line)
{
    const char* WhiteSpace = " \t\v\r\n\f";
    size_t start = line.find_first_not_of(WhiteSpace);
    size_t end = line.find_last_not_of(WhiteSpace);
    return start == end ? std::string() : line.substr(start, end - start + 1);
}

// std::string getFirstIndex(std::string indices)
// {
	
// }

const char *Config::InvalidConfigurationException::what() const throw()
{
	return "Config: Something is wrong";
}

void Config::checkConf(void)
{
	if (_port.empty())
		_port.push_back(80);
	if (_server_name.empty())
		_server_name.push_back("");
	if (_routes.empty())
		_routes["/"] = "index.html";
	std::cout << _root_dir.front() << std::endl;
	std::cout << _port.front() << std::endl;
	std::cout << _server_name.front() << std::endl;
	std::cout << _routes.begin()->first << " " << _routes.begin()->second << std::endl;
	std::cout << _root_dir.front() << std::endl;
}

void Config::locationDirective(std::ifstream& input)
{
	std::string line;
	size_t delim;
	std::string a[] = {"autoindex", "allow_methods", "root", "index"};
	std::vector<std::string> params(a, a + sizeof(a)/sizeof(std::string));
	std::vector<std::string>::iterator i;

	while (input.good())
	{
		std::getline(input, line);
		line = trim(line);
		if (line.empty())
			continue;
		if (line.find('\n') != std::string::npos)
			throw std::runtime_error("missing semicolon in location directive");
		if (input.eof() && line.length() == 0)
			throw std::runtime_error("missing closing bracket in location directive");
		for (i = params.begin(); i != params.end(); ++i)
			if ((delim = line.find(*i)) != std::string::npos)
				break;
		if (*i == "autoindex")
			std::cout << line.substr(delim) << std::endl;
		else if (*i == "allow_methods")
			std::cout << line.substr(delim) << std::endl;
		else if (*i == "root")
			std::cout << line.substr(delim) << std::endl;
		else if (*i == "index")
			std::cout << line.substr(delim) << std::endl;
	}
}

void Config::serverDirective(std::ifstream& input)
{
	std::string line;
	size_t delim;
	std::string a[] = {"server_name", "listen", "location", "root", "index"};
	std::vector<std::string> params(a, a + sizeof(a)/sizeof(std::string));
	std::vector<std::string>::iterator i;

	while (input.good())
	{
		std::getline(input, line);
		line = trim(line);
		if (line.empty())
			continue;
		if (line.find('\n') != std::string::npos)
			throw std::runtime_error("missing semicolon in server directive");
		if (input.eof() && line.length() == 0)
			throw std::runtime_error("missing closing bracket in server directive");
		for (i = params.begin(); i != params.end(); ++i)
			if ((delim = line.find(*i)) != std::string::npos)
				break;
		if (*i == "server_name")
			_server_name.push_back(line.substr(delim));
		else if (*i == "listen")
			_port.push_back(ft_stoi(line.substr(delim)));
		else if (*i == "root")
			_root_dir.push_back(line.substr(delim));
		else if (*i == "location")
			locationDirective(input);
		else if (*i == "index")
			_routes["/"] = line.substr(delim);
	}
	checkConf();
}

void Config::parseConf(std::string path)
{
	std::ifstream input(path, std::ifstream::in);
	std::string line;

	if (input.is_open() && input.good())
	{
		std::getline(input, line);
		if (line.find("server") != std::string::npos)
		{
			serverDirective(input);
		}
	}
}

Config::Config(std::string path)
{
	try
	{
		parseConf(path);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

Config::~Config()
{
	// std::cout << "Config destructed" << std::endl;
}

std::vector<size_t> Config::getPort()
{
	return _port;
}

std::vector<std::string> Config::getName()
{
	return _server_name;
}

std::vector<std::string> Config::getRoot()
{
	return _root_dir;
}

std::string Config::getIndex(std::string route)
{
	return _routes[route];
}

std::map<std::string, std::string> Config::getRoutes()
{
	return _routes;
}

