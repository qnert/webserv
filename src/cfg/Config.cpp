/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 18:12:35 by rnauke            #+#    #+#             */
/*   Updated: 2024/02/21 19:53:41 by rnauke           ###   ########.fr       */
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

// bool Config::locationExists(const std::string& uri)
// {
// 	for (std::vector<std::map<std::string, std::string> >::iterator i = _locations.begin(); i != _locations.end(); ++i)
// 	{
		
// 	}
// }

void addToMap(std::map<std::string, std::string>& map, const std::string& key, const std::string& value)
{
	if (map.find(key) != map.end())
		throw std::runtime_error("duplicate parameter");
	map.insert(std::make_pair(key, value));
}

// trim white space from beginning and end of a string
std::string trim(const std::string& line)
{
    const char* WhiteSpace = " \t\v\r\n\f";
    size_t start = line.find_first_not_of(WhiteSpace);
    size_t end = line.find_last_not_of(WhiteSpace);
    return start == end ? line : line.substr(start, end - start + 1);
}

// advances file descriptor until theres a opening bracket of a directive
void Config::findOpeningBracket(std::ifstream& input, std::string& line)
{
	while (input.good())
	{
		std::getline(input, line);
		line = trim(line);
		if (line.empty() || line.front() == '#')
			continue;
		if (line.find('{') != std::string::npos)
		{
			if (!trim(line.substr(0, line.find('{'))).empty()) // maybe false if there is "server" right before the opening bracket
				throw std::runtime_error("invalid config throw");
			return;
		}
		else
			throw std::runtime_error("missing opening bracket");
	}
}

// verifies the config and makes sure nothing is undefined
void checkLocation(std::map<std::string, std::string> map)
{
	if (map.find("autoindex") == map.end())
		map.insert(std::make_pair("listen", "80"));
	if (map.find("server_name") == map.end())
		map.insert(std::make_pair("server_name", ""));
	if (map.find("root") == map.end())
		map.insert(std::make_pair("root", "./responseFiles"));
	if (map.find("index") == map.end())
		map.insert(std::make_pair("index", "index.html"));
	
	std::cout << map.find("listen")->second << std::endl;
	std::cout << map.find("server_name")->second << std::endl;
	std::cout << map.find("root")->second << std::endl;
	std::cout << map.find("index")->second << std::endl << std::endl;
}

// verifies the config and makes sure nothing is undefined
void checkConf(std::map<std::string, std::string> map)
{
	if (map.find("listen") == map.end())
		map.insert(std::make_pair("listen", "80"));
	if (map.find("server_name") == map.end())
		map.insert(std::make_pair("server_name", ""));
	if (map.find("root") == map.end())
		map.insert(std::make_pair("root", "./responseFiles"));
	if (map.find("index") == map.end())
		map.insert(std::make_pair("index", "index.html"));
	
	std::cout << map.find("listen")->second << std::endl;
	std::cout << map.find("server_name")->second << std::endl;
	std::cout << map.find("root")->second << std::endl;
	std::cout << map.find("index")->second << std::endl << std::endl;
}

// handles parsing of the location directive
std::map<std::string,std::string> Config::locationDirective(std::ifstream& input, std::string& line)
{
	std::string a[] = {"autoindex ", "allow_methods ", "root ", "index ", "enable_cgi ", "redirect "};
	std::vector<std::string> params(a, a + sizeof(a)/sizeof(std::string));
	std::vector<std::string>::iterator i;
	std::map<std::string,std::string> map;

// figure out how to handle opening bracket on same line or different line
	std::string key = "location ";
	addToMap(map, "uri", trim(line.substr(key.length(), line.length() - key.length())));
	if (line.find('{') == std::string::npos)
		findOpeningBracket(input, line);
	while (input.good())
	{
		std::getline(input, line);
		line = trim(line);
		if (line.empty() || line.front() == '#')
			continue;
		for (i = params.begin(); i != params.end(); ++i)
		{
			if ((line.compare(0, i.base()->length(), *i)) == 0 || line.find('}') != std::string::npos)
			{
				size_t delim = line.find(';');
				if (line.find('}') < delim)
					return map;
				if (line.substr(i.base()->length(), line.length()).find(';') == std::string::npos)
					throw std::runtime_error("missing semicolon in location directive");
				addToMap(map, trim(*i), trim(line.substr(i.base()->length(), delim-i.base()->length())));
				break;
			}
			else if (next(i) == params.end())
				throw std::runtime_error("invalid argument in location directive");
		}
	}
	throw std::runtime_error("missing closing bracket in location directive");
}

// handles parsing of the server directive
std::map<std::string, std::string> Config::serverDirective(std::ifstream& input)
{
	std::string line;
	std::string a[] = {"server_name ", "listen ", "location ", "root ", "index ", "max_client_body ", "error_page "};
	std::vector<std::string> params(a, a + sizeof(a)/sizeof(std::string));
	std::vector<std::string>::iterator i;
	std::map<std::string, std::string> map;

	while (input.good())
	{
		std::getline(input, line);
		line = trim(line);
		if (line.empty() || line.front() == '#')
			continue;
		for (i = params.begin(); i != params.end(); ++i)
		{
			if ((line.compare(0, i.base()->length(), *i)) == 0 || line.find('}') != std::string::npos)
			{
				size_t delim = line.find(';');
				if (line.find('}') < delim)
					return map;
				if (trim(line.substr(0, line.find(' '))) == "location")
				{
					// if (locationExists())
					// 	continue;
					_locations.push_back(locationDirective(input, line));
					break;
				}
				else if (line.substr(i.base()->length(), line.length()).find(';') == std::string::npos)
					throw std::runtime_error("missing semicolon in server directive");
				addToMap(map, trim(*i), trim(line.substr(i.base()->length(), delim-i.base()->length())));
				break;
			}
			else if (next(i) == params.end())
				throw std::runtime_error("invalid argument server directive");
		}
	}
	return map;
}

t_confVector& Config::getLocations()
{
	return _locations;
}

std::map<std::string, std::string>& Config::getConfig()
{
	return _config;
}

bool Config::findNextServerDirective(std::ifstream& input, std::string& line)
{
	if (input.is_open() && input.good())
	{
		while (input.good())
		{
			std::getline(input, line);
			line = trim(line);
			if (line.empty() || line.front() == '#')
				continue;
			if (((line.find('{') == std::string::npos) && line == "server"))
			{
				findOpeningBracket(input, line);
				return true;
			}
			if ((line.find('{') != std::string::npos) && trim(line.substr(0, line.length()-1)) == "server")
				return true;
			// else
			// 	throw std::runtime_error("argument found outside of server directive");
		}
	}
	return false;
}

Config::Config(std::ifstream& input)
{
	std::string line;

	try
	{
		if (findNextServerDirective(input, line))
		{
			_config = serverDirective(input);
			checkConf(_config);
		}
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

