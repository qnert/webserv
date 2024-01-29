/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileupload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skunert <skunert@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 11:17:16 by skunert           #+#    #+#             */
/*   Updated: 2024/01/29 15:07:37 by skunert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>
#include <string>

size_t  ft_strlen(char* str){
  size_t len = 0;
  while (str[len])
    len++;
  return (len);
}

int main(int argc, char **argv){
  if (argc != 4)
    return (1);
  std::string path = argv[2];
  std::ofstream upload(("./responseFiles/Upload/" + path).c_str(), std::ios::binary);
  if (!upload.is_open())
    return (1);
  upload.write(argv[3], ft_strlen(argv[3]));
  upload.close();
  return (0);
}
