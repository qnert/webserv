#!/usr/bin/env python3

ascii_art="kkkkkkkk                                  tttt\nk::::::k                                ttt:::t\nk::::::k                                t:::::t\nk::::::k                                t:::::t\nk:::::k    kkkkkkk ooooooooooo   ttttttt:::::ttttttt\nk:::::k   k:::::koo:::::::::::oo t:::::::::::::::::t\nk:::::k  k:::::ko:::::::::::::::ot:::::::::::::::::t\nk:::::k k:::::k o:::::ooooo:::::otttttt:::::::tttttt\nk::::::k:::::k  o::::o     o::::o      t:::::t\nk:::::::::::k   o::::o     o::::o      t:::::t\nk:::::::::::k   o::::o     o::::o      t:::::t\nk::::::k:::::k  o::::o     o::::o      t:::::t    tttttt\nk::::::k k:::::k o:::::ooooo:::::o      t::::::tttt:::::t\nk::::::k  k:::::ko:::::::::::::::o      tt::::::::::::::t\nk::::::k   k:::::koo:::::::::::oo         tt:::::::::::tt\nkkkkkkkk    kkkkkkk ooooooooooo             ttttttttttt\n"


print("Content-type: text/plain")
print("Set-Cookie: secret=kot; Max-Age=3630")
print("Content-length: {}\r\n\r\n".format(len(ascii_art)))
print(ascii_art)


