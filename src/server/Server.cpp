#include "../../includes/Server.hpp"

Server::Server()
{
  if ((this->serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    throw(std::runtime_error("Error getting socket"));

  this->serverAdress.sin_family = AF_INET;
  this->serverAdress.sin_addr.s_addr = INADDR_ANY;
  this->serverAdress.sin_port = htons(PORT);

  // associates the server socket with the local address
  // and port specified in the "serverAddress" structure
  if (bind(this->serverSocket, reinterpret_cast<struct sockaddr*>(&serverAdress), sizeof(serverAdress)) == -1) {
    close(this->serverSocket);
    throw(std::runtime_error("Error binding socket"));
  }

  // listens for incomming connection requests
  if (listen(this->serverSocket, SOMAXCONN) == -1) {
    close(this->serverSocket);
    throw(std::runtime_error("Error listening for connections"));
  }
}

Server::~Server()
{
  close(this->serverSocket);
}

void  Server::serverLoop()
{
  while (true)
  {
    if ((this->clientSocket = accept(this->serverSocket, NULL, NULL)) == -1) {
      const std::string errorResponse = "503 Service Unavailable\n";
      send(this->clientSocket, errorResponse.c_str(), errorResponse.size(), 0);
      close(this->clientSocket);
      continue;
    }
    const std::string html_content =
    "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "<head>\n"
    "    <meta charset=\"UTF-8\">\n"
    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    "    <title>Cool UI Example</title>\n"
    "    <style>\n"
    "        body {\n"
    "            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;\n"
    "            background-color: #f4f4f4;\n"
    "            margin: 0;\n"
    "            padding: 0;\n"
    "            display: flex;\n"
    "            align-items: center;\n"
    "            justify-content: center;\n"
    "            height: 100vh;\n"
    "        }\n"
    "        .cool-container {\n"
    "            background-color: #fff;\n"
    "            border-radius: 10px;\n"
    "            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);\n"
    "            padding: 20px;\n"
    "            text-align: center;\n"
    "            max-width: 400px;\n"
    "            width: 100%;\n"
    "        }\n"
    "        h1 {\n"
    "            color: #3498db;\n"
    "        }\n"
    "        p {\n"
    "            color: #555;\n"
    "            line-height: 1.6;\n"
    "        }\n"
    "        button {\n"
    "            background-color: #3498db;\n"
    "            color: #fff;\n"
    "            border: none;\n"
    "            padding: 10px 20px;\n"
    "            font-size: 16px;\n"
    "            cursor: pointer;\n"
    "            border-radius: 5px;\n"
    "            transition: background-color 0.3s ease;\n"
    "        }\n"
    "        button:hover {\n"
    "            background-color: #2980b9;\n"
    "        }\n"
    "    </style>\n"
    "</head>\n"
    "<body>\n"
    "    <div class=\"cool-container\">\n"
    "        <h1>Welcome to Webserv!</h1>\n"
    "        <p>Please click to get bitches.</p>\n"
    "        <button>Click Me</button>\n"
    "    </div>\n"
    "</body>\n"
    "</html>\n";
    send(this->clientSocket, html_content.c_str(), html_content.size(), 0);
  }
  close(this->clientSocket);
}
