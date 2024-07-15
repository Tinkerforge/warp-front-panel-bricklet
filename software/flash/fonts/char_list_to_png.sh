#!/bin/bash
convert -size 50:3600 xc:white -font "FreeMono" -pointsize 16 -fill black -draw @char_list.txt 0.png
convert -size 50x3600 xc:black -font "FreeMono" -pointsize 16 -fill white -draw @char_list.txt 1.png
convert -size 50x3600 xc:'#003067' -font "FreeMono" -pointsize 16 -fill white -draw @char_list.txt 2.png
convert -size 50x3600 xc:white -font "FreeMono" -pointsize 18 -fill black -draw @char_list.txt 3.png
convert -size 50x3600 xc:black -font "FreeMono" -pointsize 18 -fill white -draw @char_list.txt 4.png
convert -size 50x3600 xc:'#003067' -font "FreeMono" -pointsize 18 -fill white -draw @char_list.txt 5.png
convert -size 50x3600 xc:white -font "FreeMono" -pointsize 20 -fill black -draw @char_list.txt 6.png
convert -size 50x3600 xc:black -font "FreeMono" -pointsize 20 -fill white -draw @char_list.txt 7.png
convert -size 50x3600 xc:'#003067' -font "FreeMono" -pointsize 20 -fill white -draw @char_list.txt 8.png

#IFS=" x+" read a b c d < <(convert -fuzz 10% font_16pt_freemono_black_on_white.png -format "%@" info:)
#convert font_16pt_freemono_black_on_white.png -crop "${a}x+${c}+0" font_16pt_freemono_black_on_white.png

convert -trim 0.png 0.png
convert -trim 1.png 1.png
convert -trim 2.png 2.png
convert -trim 3.png 3.png
convert -trim 4.png 4.png
convert -trim 5.png 5.png
convert -trim 6.png 6.png
convert -trim 7.png 7.png
convert -trim 8.png 8.png
