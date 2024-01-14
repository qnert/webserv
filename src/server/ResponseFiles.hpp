/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseFiles.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:23 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/13 16:33:03 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

class ResponseFiles
{
private:
  std::map<std::string, std::string> _responseFiles; // stores response files with name as key and content as value
public:
  ResponseFiles();
  ~ResponseFiles();

  const std::map<std::string, std::string>& getResponseFiles() const;

  void  storeFileIntoMap(const std::string& name, const std::string& path);
};
