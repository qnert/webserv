/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 16:11:27 by rnauke            #+#    #+#             */
/*   Updated: 2024/02/01 18:20:17 by rnauke           ###   ########.fr       */
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

// trim white space from beginning and end of a string
std::string trim(const std::string& line)
{
    const char* WhiteSpace = " \t\v\r\n\f";
    size_t start = line.find_first_not_of(WhiteSpace);
    size_t end = line.find_last_not_of(WhiteSpace);
    return start == end ? std::string() : line.substr(start, end - start + 1);
}

// advances file descriptor until theres a opening bracket of a directive
void findOpeningBracket(std::ifstream& input)
{
	std::string line;

	while (input.good())
	{
		std::getline(input, line);
		size_t found = line.find_first_not_of(" \t\v\r\n\f");
		if (found != std::string::npos && line.at(found) == '{')
		{
			return;
		}
		else if (input.eof())
			throw std::runtime_error("missing opening bracket");
	}
}

// check if argument line ends with a semicolon
int checkSemicolon(std::vector<std::string> validArgs, std::string line)
{
	std::vector<std::string>::iterator i;
	for (i = validArgs.begin(); i != validArgs.end(); ++i)
		if ((line.find(*i)) != std::string::npos)
			break;
	if (*i == "server_name ")
		return 1;
	else if (*i == "listen ")
		return 1;
	else if (*i == "root ")
		return 1;
	else if (*i == "location ")
		return 1;
	else if (*i == "index ")
		return 1;
	return 0;
}

const char *Config::InvalidConfigurationException::what() const throw()
{
	return "Config: Something is wrong";
}

// verifies the config and makes sure nothing is undefined
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

// handles parsing of the server directive
void Config::locationDirective(std::ifstream& input)
{
	// go until find valid argument for server directive
	// if anything goes wrong throw (missing semicolon/closing bracket/invalid argument)
	// basically just copy serverDirective() and change array of valid args
	std::string line;
	size_t delim;
	std::string a[] = {"autoindex ", "allow_methods ", "root ", "index "};
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
		if (*i == "autoindex ")
			std::cout << line.substr(delim) << std::endl;
		else if (*i == "allow_methods ")
			std::cout << line.substr(delim) << std::endl;
		else if (*i == "root ")
			std::cout << line.substr(delim) << std::endl;
		else if (*i == "index ")
			std::cout << line.substr(delim) << std::endl;
	}
}

// handles parsing of the server directive
void Config::serverDirective(std::ifstream& input)
{
	std::string line;
	std::string a[] = {"server_name ", "listen ", "location ", "root ", "index "};
	std::vector<std::string> params(a, a + sizeof(a)/sizeof(std::string));
	std::vector<std::string>::iterator i;

	while (input.good())
	{
		// if eof is reached without finding closing bracket throw missing bracket in server directive 
		std::getline(input, line, ';');
		line = trim(line);
		if (line.empty())
			continue;
		for (i = params.begin(); i != params.end(); ++i)
			if ((line.compare(0, i.base()->length(), *i)) == 0)
				break;
		if (checkSemicolon(params, line.substr(i.base()->length())))
			throw std::runtime_error("missing semicolon in server directive");
		if (*i == "server_name ")
			_server_name.push_back(trim(line.substr(i.base()->length())));
		else if (*i == "listen ")
			_port.push_back(ft_stoi(trim(line.substr(i.base()->length()))));
		else if (*i == "root ")
			_root_dir.push_back(trim(line.substr(i.base()->length())));
		else if (*i == "location ")
			locationDirective(input);
		else if (*i == "index ")
			_routes["/"] = trim(line.substr(i.base()->length()));
		else
			throw std::runtime_error("invalid argument");
	}
	checkConf();
}

void Config::parseConf(std::string path)
{
	std::ifstream input(path, std::ifstream::in);
	std::string line;

	if (input.is_open() && input.good())
	{
		while (input.good())
		{
			std::getline(input, line);
			if (line.find("server") != std::string::npos)
			{
				if (line.find('{') == std::string::npos)
					findOpeningBracket(input);
				serverDirective(input);
			}
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

