/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 15:10:35 by njantsch          #+#    #+#             */
/*   Updated: 2024/01/16 10:08:23 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/ResponseFiles.hpp"
#include "../includes/MIME_type.hpp"

int main(void)
{
  try
  {
    MIME_type     data_types;
    ResponseFiles files;
    files.storeFileIntoMap("index", "./responseFiles/index.html", data_types);
    files.storeFileIntoMap("image", "./responseFiles/image.webp", data_types);
    files.storeFileIntoMap("error", "./responseFiles/error.html", data_types);
    files.storeFileIntoMap("backg", "./responseFiles/background.webp", data_types);
    Server httpServer(files);
    httpServer.serverLoop();
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << '\n';
  }
  return (0);
}
