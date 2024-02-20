/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:35 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/20 15:07:17 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerManager.hpp"
#include "../includes/MIME_type.hpp"
#include "../includes/Statuscodes.hpp"
#include "../includes/Config.hpp"

int main(int argc, char** argv)
{
  if (argc > 2)
    return 1;
  try
  {
    std::string path;
	  if (argc < 2) {
      (void) argv;
      path = "config/default.conf";
    }
    else
      path = argv[1];
    Config cfg(path);
    ServerManager servers(cfg);
    servers.serverLoop();
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << '\n';
  }
  return (0);
}
