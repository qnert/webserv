# C++ HTTP Web Server

## Description
This project is an HTTP web server implemented in C++98, designed to serve static websites, handle various HTTP methods such as GET, POST, and DELETE, and support file uploads. The server ensures non-blocking I/O operations and accurate handling of HTTP response status codes and error pages.

## Usage
```bash
$ git clone [https://github.com/your_username/http-web-server.git](https://github.com/qnert/webserv.git)
$ cd webserv
$ ## Make use of the Makefile
$ make
$ ./webserv ./path/to/configfile
```

## Configuration File
The server requires a configuration file (e.g., config.ini) specifying the server settings, such as ports to listen on, document root directory, etc.
By referencing NGINX configuration, users familiar with NGINX can quickly grasp the structure and syntax of our server's configuration file and configure it according to their requirements.

## Testing
The server can handle stress tests to stay available even under heavy loads of requests as well as refering to NGINX to provide similar functionalities.

## Credits
This project was developed by:
- [Nico](https://github.com/njantsch)
- [Simon](https://github.com/qnert)
- [Rouven](https://github.com/Narouv)

