/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:35 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/23 13:08:21 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/ResponseFiles.hpp"
#include "../includes/MIME_type.hpp"
#include "../includes/Statuscodes.hpp"

int main(void)
{
  try
  {
    MIME_type     data_types;
    Statuscodes   statuscodes;
    Server httpServer;
    httpServer.serverLoop(data_types, statuscodes);
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << '\n';
  }
  return (0);
}
