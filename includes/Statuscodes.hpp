/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Statuscodes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 09:26:18 by skunert           #+#    #+#             */
/*   Updated: 2024/01/16 09:47:12 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <fstream>
#include <iostream>

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
