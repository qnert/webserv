/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MIME_type.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 09:30:09 by skunert           #+#    #+#             */
/*   Updated: 2024/01/16 09:59:50 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/MIME_type.hpp"

std::string const&  MIME_type::operator[](std::string const& key){
  return (this->data[key]);
}

MIME_type::MIME_type(){
  std::string buff;
  std::ifstream file("./data/mime.csv");

  if (!file.is_open())
    std::runtime_error("Coudln't open mime.csv!\n");
  std::getline(file, buff);
  if (buff != "Name|MIME_typeType")
    std::runtime_error("Invalid header in mime.csv!\n");
  while (std::getline(file, buff)){
    this->data[buff.substr(0, buff.find_first_of("|"))] = buff.substr(buff.find_first_of("|") + 1, buff.size() - 1);
  }
  file.close();
}

MIME_type::MIME_type(MIME_type const& other){
  *this = other;
}

MIME_type& MIME_type::operator=(MIME_type const& other){
  this->data = other.data;
  return (*this);
}

MIME_type::~MIME_type(){}
