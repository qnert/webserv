/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 14:35:15 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/11 19:26:41 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void  Server::cleanUpClientFds()
{
  for (size_t i = 0; i < this->_nfds; i++)
    close(this->_clientPollfds[i].fd);
}

void  Server::removeAndCompressFds(int i)
{
  close(this->_clientPollfds[i].fd);
  for (size_t j = i; j != 0 && j < this->_nfds - 1; j++)
  {
    this->_clientPollfds[j] = this->_clientPollfds[j + 1];
  }
  this->_nfds--;
}

std::string Server::ft_itos(size_t num)
{
  std::ostringstream oss;
  oss << num;
  return (oss.str());
}

MIME_type& Server::getMimeType(void) {return (this->_data);}

Statuscodes& Server::getStatuscodes(void) {return (this->_codes);}
