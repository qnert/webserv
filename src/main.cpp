/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:35 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/14 19:44:52 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/ResponseFiles.hpp"

int main(void)
{
  try
  {
    ResponseFiles files;
    files.storeFileIntoMap("index", "./responseFiles/index.html");
    // files.storeFileIntoMap("giphy", "./responseFiles/Donquixote_Doflamingo_render.webp");
    Server httpServer(files);
    httpServer.serverLoop();
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << '\n';
  }
  return (0);
}
