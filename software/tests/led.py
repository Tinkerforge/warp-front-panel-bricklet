#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
import datetime
import time
from common import get_first_warp_front_panel_bricklet
import random


if __name__ == "__main__":
    wfp = get_first_warp_front_panel_bricklet()

    while True:
        wfp.set_led_state(wfp.LED_PATTERN_ON, wfp.LED_COLOR_RED)
        print(wfp.get_led_state())
        time.sleep(1)
        wfp.set_led_state(wfp.LED_PATTERN_ON, wfp.LED_COLOR_GREEN)
        print(wfp.get_led_state())
        time.sleep(1)
        wfp.set_led_state(wfp.LED_PATTERN_ON, wfp.LED_COLOR_YELLOW)
        print(wfp.get_led_state())
        time.sleep(1)
        wfp.set_led_state(wfp.LED_PATTERN_BLINKING, wfp.LED_COLOR_RED)
        print(wfp.get_led_state())
        time.sleep(10)
        wfp.set_led_state(wfp.LED_PATTERN_BLINKING, wfp.LED_COLOR_GREEN)
        print(wfp.get_led_state())
        time.sleep(10)
        wfp.set_led_state(wfp.LED_PATTERN_BLINKING, wfp.LED_COLOR_YELLOW)
        print(wfp.get_led_state())
        time.sleep(10)
        wfp.set_led_state(wfp.LED_PATTERN_BREATHING, wfp.LED_COLOR_RED)
        print(wfp.get_led_state())
        time.sleep(10)
        wfp.set_led_state(wfp.LED_PATTERN_BREATHING, wfp.LED_COLOR_GREEN)
        print(wfp.get_led_state())
        time.sleep(10)
        wfp.set_led_state(wfp.LED_PATTERN_BREATHING, wfp.LED_COLOR_YELLOW)
        print(wfp.get_led_state())
        time.sleep(10)
