#!/usr/bin/env python3

string = "42 inside of you"
print('Content-Type: text/plain')
print('Content-Length: ', len(string) + 1,'\n\n')
print(string)
