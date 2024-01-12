#include "./server/Server.hpp"

int main(int argc, char **argv){
  if (argc > 2 && argv != NULL)
    return (std::cerr << "False amount of arguments!" << std::endl, 1);
  int new_socket;
  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(0x7f000001);
  address.sin_port = htons(8080);
  int addrlen = sizeof(address);
  Server server(address);
  char coolHtml[] = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<!DOCTYPE html>\n"
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
  while (1){
    if ((new_socket = accept(server.getFd(), (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0){
        return (std::cerr << "Error: accept function failed!" << std::endl, 1);
      }
    if (send(new_socket, coolHtml, sizeof(coolHtml), 0) == -1){
      return (std::cerr << "Error: send function failed!" << std::endl, 1);
    }
  }
}
