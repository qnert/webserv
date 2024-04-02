# C++ HTTP Web Server

## Description
This project is an HTTP web server implemented in C++98, designed to serve static websites, handle various HTTP methods such as GET, POST, and DELETE, and support file uploads. The server ensures non-blocking I/O operations and accurate handling of HTTP response status codes and error pages.

## Usage
```bash
$ git clone https://github.com/your_username/http-web-server.git
$ cd http-web-server
$ g++ -std=c++98 -o server server.cpp
$ ./server config.ini
