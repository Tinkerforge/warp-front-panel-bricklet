#!/bin/bash
convert -size 50:3600 xc:white     -font "DejaVu-Sans-Mono-Bold" -pointsize 16 -fill black -draw @char_list.txt -trim 0.png
convert -size 50x3600 xc:black     -font "DejaVu-Sans-Mono-Bold" -pointsize 16 -fill white -draw @char_list.txt -trim 1.png
convert -size 50x3600 xc:'#003067' -font "DejaVu-Sans-Mono-Bold" -pointsize 16 -fill white -draw @char_list.txt -trim 2.png
convert -size 50x3600 xc:white     -font "DejaVu-Sans-Mono-Bold" -pointsize 18 -fill black -draw @char_list.txt -trim 3.png
convert -size 50x3600 xc:black     -font "DejaVu-Sans-Mono-Bold" -pointsize 18 -fill white -draw @char_list.txt -trim 4.png
convert -size 50x3600 xc:'#003067' -font "DejaVu-Sans-Mono-Bold" -pointsize 18 -fill white -draw @char_list.txt -trim 5.png
convert -size 50x3600 xc:white     -font "DejaVu-Sans-Mono-Bold" -pointsize 20 -fill black -draw @char_list.txt -trim 6.png
convert -size 50x3600 xc:black     -font "DejaVu-Sans-Mono-Bold" -pointsize 20 -fill white -draw @char_list.txt -trim 7.png
convert -size 50x3600 xc:'#003067' -font "DejaVu-Sans-Mono-Bold" -pointsize 20 -fill white -draw @char_list.txt -trim 8.png

#IFS=" x+" read a b c d < <(convert -fuzz 10% font_16pt_freemono_black_on_white.png -format "%@" info:)
#convert font_16pt_freemono_black_on_white.png -crop "${a}x+${c}+0" font_16pt_freemono_black_on_white.png
