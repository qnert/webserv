/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnauke <rnauke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:35 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/22 14:31:24 by rnauke           ###   ########.fr       */
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
	if (argc < 2)
		path = "config/default.conf";
    else
      path = argv[1];
    ServerManager servers(path);
    // servers.serverLoop();
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << '\n';
  }
  return (0);
}
