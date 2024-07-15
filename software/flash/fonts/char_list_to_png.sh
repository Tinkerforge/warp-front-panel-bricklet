#!/bin/sh
convert -size 3600x3600 xc:white -font "FreeMono" -pointsize 16 -fill black -draw @char_list.txt font_16pt_freemono.png
convert -trim font_16pt_freemono.png font_16pt_freemono.png
