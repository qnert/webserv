#!/usr/bin/env python3

import os

path_info = os.environ.get("PATH_INFO", "")
if path_info == "/":
  path_info = "No input given\n"

print('Content-Type: text/plain')
print('Content-Length: ', len(path_info),'\n')
print(path_info)
