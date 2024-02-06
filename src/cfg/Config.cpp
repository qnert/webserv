/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 18:12:35 by rnauke            #+#    #+#             */
/*   Updated: 2024/02/06 18:20:37 by rnauke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/Config.hpp"

// static int ft_stoi(const std::string& str)
// {
// 	int num;
// 	std::stringstream ss(str);

// 	ss >> num;
// 	return num;
// }

// trim white space from beginning and end of a string
std::string trim(const std::string& line)
{
    const char* WhiteSpace = " \t\v\r\n\f";
    size_t start = line.find_first_not_of(WhiteSpace);
    size_t end = line.find_last_not_of(WhiteSpace);
    return start == end ? std::string("") : line.substr(start, end - start + 1);
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

// verifies the config and makes sure nothing is undefined
void checkConf(std::map<std::string, std::string> map)
{
	std::cout << map.find("listen")->second << std::endl;
	std::cout << map.find("server_name")->second << std::endl;
	std::cout << map.find("root")->second << std::endl;
	std::cout << map.find("index")->second << std::endl;
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
			std::cout << /*line.substr(delim)*/ "found something :)" << std::endl;
		else if (*i == "allow_methods ")
			std::cout << /*line.substr(delim)*/ "found something :)" << std::endl;
		else if (*i == "root ")
			std::cout << /*line.substr(delim)*/ "found something :)" << std::endl;
		else if (*i == "index ")
			std::cout << /*line.substr(delim)*/ "found something :)" << std::endl;
	}
}

// handles parsing of the server directive
std::map<std::string, std::string> Config::serverDirective(std::ifstream& input)
{
	std::string line;
	std::string a[] = {"server_name ", "listen ", "location ", "root ", "index "};
	std::vector<std::string> params(a, a + sizeof(a)/sizeof(std::string));
	std::vector<std::string>::iterator i;
	std::map<std::string, std::string> map;

	while (input.good())
	{
		// if eof is reached without finding closing bracket throw missing bracket in server directive 
		std::getline(input, line); // rewrite to only read until newline to deal with comments easier. check for semicolon at end of each line and remove any line with # in beginning
		line = trim(line);
		if (line.empty() || line.front() == '#')
			continue;
		for (i = params.begin(); i != params.end(); ++i)
		{
			if ((line.compare(0, i.base()->length(), *i)) == 0)
			{
				size_t delim = line.find(';');
				if (line.substr(i.base()->length(), line.length()).find(';') == std::string::npos)
					throw std::runtime_error("missing semicolon in server directive");
				map[trim(*i)] = trim(line.substr(i.base()->length(), delim-i.base()->length()));
				break;
			}
			else if (next(i) == params.end())
				throw std::runtime_error("invalid argument");
		}
	}
	checkConf(map);
	return map;
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
				// fix opening bracket not being found if on same line as server directive start
					findOpeningBracket(input);
				_configs.push_back(serverDirective(input));
			}
		}
	}
}

Config::Config(const std::string& path)
{
	try
	{
		parseConf(path);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << "exit here" << '\n';
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
