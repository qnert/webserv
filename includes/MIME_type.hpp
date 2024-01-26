/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MIME_type.hpp                                      :+:      :+:    :+:   */
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

class MIME_type{
  private:
    std::map<std::string, std::string>  data;

  public:
    std::string const&  operator[](std::string const& key);

  MIME_type(void);
  MIME_type(MIME_type const& other);
  MIME_type& operator=(MIME_type const& other);
  ~MIME_type(void);
};
