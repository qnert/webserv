/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 15:47:41 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/23 17:09:55 by rnauke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Utils.hpp"

short ft_stosh(const std::string& str)
{
	short num;
	std::stringstream ss(str);

	ss >> num;
	return num;
}

size_t ft_stosize(const std::string& str)
{
	size_t num;
	std::stringstream ss(str);

	ss >> num;
	return num;
}

std::string ft_itos(size_t num)
{
  std::ostringstream oss;
  oss << num;
  return (oss.str());
}

long long ft_stoll(const std::string& str)
{
	long long num;
	std::stringstream ss(str);

	ss >> num;
	return num;
}