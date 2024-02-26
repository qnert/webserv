/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProcessResponse.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njantsch <njantsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 15:23:23 by njantsch          #+#    #+#             */
/*   Updated: 2024/02/26 10:19:25 by njantsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void  Server::chooseMethod(size_t idx)
{
  static std::string tmp;
  const std::string requestType = this->_clientDetails[idx].getRequestType();

  if (this->_clientDetails[idx].getConStatus() == CLOSE)
    return;

  if (this->_clientDetails[idx].getMapValue("Version") != "HTTP/1.1") {
    versionNotSupported(idx);
    return;
  }

  if ((this->_currLocation.find("max_client_body") != this->_currLocation.end()
      && this->_clientDetails[idx].getBodySize() > ft_stosize(this->_currLocation.find("max_client_body")->second))
      || (this->_clientDetails[idx].getBodySize() > this->_maxClientBody)) {
    payloadTooLarge(idx);
    return;
  }

  if (requestType == "HEAD") {
    methodNotAllowed(idx);
  return;
  }
  if (requestType == "GET" && !checkLocationPrelims("GET")) {
    getMethod(idx, tmp);
    return;
  }

  if (requestType == "POST" && !checkLocationPrelims("POST")) {
    if (postMethod(idx) != 0)
      methodNotAllowed(idx);
    return;
  }

  if (requestType == "DELETE" && !checkLocationPrelims("DELETE")) {
    tmp = handle_file_erasing(this->_clientPollfds[idx].fd, this->_clientDetails[idx], this->_codes);
    return;
  }

  if (checkLocationPrelims(requestType) \
      && this->_currLocation["deny_methods"].find(requestType) == std::string::npos) {
    handleRedirectLocation(idx);
    return ;
  }

  if (requestType != "GET" && requestType != "POST" && requestType != "DELETE") {
    notImplemented(idx);
    return;
  }

    methodNotAllowed(idx);
}

// sends an answer to the client
void  Server::sendAnswer(size_t idx, nfds_t& nfds)
{
  this->getCurrLocation(idx);
  this->setRightCurrDir(idx);
  this->chooseMethod(idx);

  if (this->_clientDetails[idx].getMapValue("Connection") != "keep-alive"
      || this->_clientDetails[idx].getConStatus() == CLOSE) {
    std::cout << "answer sent at idx: " << idx << " set back to POLLIN" << std::endl;
    this->removeFd(idx, nfds);
  }
  else if (this->_clientDetails[idx].getPendingResponse() == false) {
    this->_clientDetails[idx].cleanUp();
    this->_clientDetails[idx].cleanUpResponse();
    this->_clientDetails[idx].refreshTime(std::time(NULL));
    this->_clientPollfds[idx].events = POLLIN;
  }
}
