/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:35 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/07 15:49:05 by rnauke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/MIME_type.hpp"
#include "../includes/Statuscodes.hpp"
#include "../includes/Config.hpp"

int main(int argc, char** argv)
{
  try
  {
    // MIME_type     data_types;
    // Statuscodes   statuscodes;
    // Server httpServer;
	if (argc == 2)
		Config cfg = Config(argv[1]);
    // httpServer.serverLoop(data_types, statuscodes);
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << '\n';
  }
  return (0);
}
