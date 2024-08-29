#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
import datetime
import time
from common import get_first_warp_front_panel_bricklet
import random


if __name__ == "__main__":
    wfp = get_first_warp_front_panel_bricklet()

    wfp.set_display(wfp.DISPLAY_AUTOMATIC)
    while True:
        print(wfp.get_display())
        time.sleep(1)
