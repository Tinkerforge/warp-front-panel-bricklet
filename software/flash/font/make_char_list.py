#!/usr/bin/env python3
# -*- coding: utf-8 -*-

s = 'text 0,0 \"\n'

extra_chars = ['☂', '☁', '☀', '→', '↓', '←', '↑', '↺', 'ß', 'ẞ', 'ä', 'Ä', 'ö', 'Ö', 'ü', 'Ü', '€']

extra_chars = ['█'] + [' ']*(32-len(extra_chars)) + extra_chars

for i, c in enumerate(extra_chars):
    print(str(i) + ' "' + c + '"')
    s += c + '\n'

for i in range(33, 127):
    print(str(i) + ' "' + chr(i) + '"')
    if i == 34:
        s += '\\' + chr(i) + '\n'
    else: 
        s += chr(i) + '\n'

print(str(128) + ' "' + extra_chars[0] + '"')
s += extra_chars[0] + '\n'
s += '"'

open('char_list.txt', 'w').write(s)
