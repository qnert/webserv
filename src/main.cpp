/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:35 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/14 16:28:26 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/Server.hpp"
#include "./server/ResponseFiles.hpp"

int main(void)
{
  try
  {
    ResponseFiles files;
    files.storeFileIntoMap("index", "./responseFiles/index.html");
    files.storeFileIntoMap("giphy", "./responseFiles/giphy.gif");
    Server httpServer(files);
    httpServer.serverLoop();
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << '\n';
  }
  return (0);
}
