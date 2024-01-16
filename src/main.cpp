/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:35 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/16 12:01:47 by skunert          ###   ########.fr       */
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
    ResponseFiles files;
    files.storeFileIntoMap("/", "./responseFiles/index.html");
    files.storeFileIntoMap("/image.webp", "./responseFiles/image.webp");
    files.storeFileIntoMap("error", "./responseFiles/error.html");
    files.storeFileIntoMap("/background.webp", "./responseFiles/background.webp");
    Server httpServer(files);
    httpServer.serverLoop(data_types, statuscodes);
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << '\n';
  }
  return (0);
}
