#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
import datetime
import time
from common import get_first_warp_front_panel_bricklet
import random

def fill_10(s):
    return s + "\0"*(10-len(s))

if __name__ == "__main__":
    wfp = get_first_warp_front_panel_bricklet()

    wfp.set_display_front_page_icon(0, True, 0, fill_10("PV"), 0, fill_10("22 kW"), 0);
    wfp.set_display_front_page_icon(1, True, 1, fill_10("Box 1"), 0, fill_10("9,6 kW"), 0);
    wfp.set_display_front_page_icon(2, True, 1, fill_10("Box 2"), 0, fill_10("2,8 kW"), 0);
    wfp.set_display_front_page_icon(3, True, 2, fill_10("Preis"), 0, fill_10("0,18 {0}".format(chr(31))), 0);
    wfp.set_display_front_page_icon(4, True, 3, fill_10("Not"), 0, fill_10("Locked"), 0);
    wfp.set_display_front_page_icon(5, True, 4, fill_10("Config"), 0, fill_10("OK"), 0);

    while True:
        d = datetime.datetime.now()
        wfp.set_status_bar(0, 0, d.hour, d.minute, d.second)
        val1 = random.randint(19, 22)
        val2 = random.randint(59, 99) / 10
        val3 = random.randint(10, 59) / 10
        val4 = random.randint(10, 19)
        wfp.set_display_front_page_icon(0, True, 0, fill_10("PV"), 0, fill_10("{0} kW".format(val1)), 0);
        wfp.set_display_front_page_icon(1, True, 1, fill_10("Box 1"), 0, fill_10("{0} kW".format(val2)), 0);
        wfp.set_display_front_page_icon(2, True, 1, fill_10("Box 2"), 0, fill_10("{0} kW".format(val3)), 0);
        wfp.set_display_front_page_icon(3, True, 2, fill_10("Preis"), 0, fill_10("0,{0} {1}".format(val4, chr(31))), 0);

        time.sleep(1)
