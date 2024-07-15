#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
import datetime
import time
from common import get_first_warp_front_panel_bricklet
import random

def fill_10(s):
    return s + "\0"*(10-len(s))

FONT_INDEX = 7

if __name__ == "__main__":
    wfp = get_first_warp_front_panel_bricklet()

    wfp.set_display_front_page_icon(0, True, 0, fill_10("PV"), FONT_INDEX, fill_10("22 kW"), FONT_INDEX);
    wfp.set_display_front_page_icon(1, True, 1, fill_10("Box 1"), FONT_INDEX, fill_10("9,6 kW"), FONT_INDEX);
    wfp.set_display_front_page_icon(2, True, 1, fill_10("Box 2"), FONT_INDEX, fill_10("2,8 kW"), FONT_INDEX);
    wfp.set_display_front_page_icon(3, True, 2, fill_10("Preis"), FONT_INDEX, fill_10("0,18 {0}".format(chr(31))), FONT_INDEX);
    wfp.set_display_front_page_icon(4, True, 3, fill_10("Not"), FONT_INDEX, fill_10("Locked"), FONT_INDEX);
    wfp.set_display_front_page_icon(5, True, 4, fill_10("Config"), FONT_INDEX, fill_10("OK"), FONT_INDEX);

    while True:
        d = datetime.datetime.now()
        wfp.set_status_bar(0, 0, d.hour, d.minute, d.second)
        val1 = random.randint(19, 22)
        val2 = random.randint(59, 99) / 10
        val3 = random.randint(10, 59) / 10
        val4 = random.randint(10, 19)
        wfp.set_display_front_page_icon(0, True, 0, fill_10("PV"), FONT_INDEX, fill_10("{0} kW".format(val1)), FONT_INDEX);
        wfp.set_display_front_page_icon(1, True, 1, fill_10("Box 1"), FONT_INDEX, fill_10("{0} kW".format(val2)), FONT_INDEX);
        wfp.set_display_front_page_icon(2, True, 1, fill_10("Box 2"), FONT_INDEX, fill_10("{0} kW".format(val3)), FONT_INDEX);
        wfp.set_display_front_page_icon(3, True, 2, fill_10("Preis"), FONT_INDEX, fill_10("0,{0} {1}".format(val4, chr(31))), FONT_INDEX);

        time.sleep(1)
