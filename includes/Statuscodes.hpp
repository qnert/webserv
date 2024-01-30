/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Statuscodes.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 09:26:18 by skunert           #+#    #+#             */
/*   Updated: 2024/01/29 14:19:19 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <fstream>
#include <iostream>
#include <cstdlib>

class Statuscodes{
  private:
    std::map<int, std::string>  data;

  public:
    std::string const&  operator[](int const& key);

  Statuscodes(void);
  Statuscodes(Statuscodes const& other);
  Statuscodes& operator=(Statuscodes const& other);
  ~Statuscodes(void);
};
