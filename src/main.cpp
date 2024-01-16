/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:35 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/16 11:24:15 by skunert          ###   ########.fr       */
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
    files.storeFileIntoMap("index", "./responseFiles/index.html", data_types, statuscodes);
    files.storeFileIntoMap("image", "./responseFiles/image.webp", data_types, statuscodes);
    files.storeFileIntoMap("error", "./responseFiles/error.html", data_types, statuscodes);
    files.storeFileIntoMap("backg", "./responseFiles/background.webp", data_types, statuscodes);
    Server httpServer(files);
    httpServer.serverLoop();
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << '\n';
  }
  return (0);
}
