#!/usr/bin/env python3

import os
import urllib.parse

def parse_query_string(query_string):
    parsed_query = urllib.parse.parse_qs(query_string)
    return parsed_query

def main():
    # Get the QUERY_STRING from environment variables
    query_string = os.environ.get("QUERY_STRING", "")

    # Parse the QUERY_STRING
    parsed_query = parse_query_string(query_string)

    # Extract user_name and password parameters
    user_name = parsed_query.get("user_name", [""])[0]
    password = parsed_query.get("password", [""])[0]

    # Print HTML content
    print("Content-type: text/html\r\n\r\n")
    print("<html>")
    print('<head><title>User Authentication</title></head>')
    print("<body>")
    print("<h1>User Authentication</h1>")
    if user_name and password:
        print("<p>Welcome, {}! Your password is: {}</p>".format(user_name, password))
    else:
        print("<p>No user name or password provided.</p>")
    print("</body>")
    print("</html>")

if __name__ == "__main__":
    main()
