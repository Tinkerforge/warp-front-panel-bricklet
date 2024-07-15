#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
import datetime
import time
from common import get_first_warp_front_panel_bricklet

if __name__ == "__main__":
    wfp = get_first_warp_front_panel_bricklet()

    while True:
        d = datetime.datetime.now()
        wfp.set_status_bar(0, 0, d.hour, d.minute, d.second)
        time.sleep(1)
