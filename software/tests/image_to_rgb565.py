#!/usr/bin/env python3
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "U1sxF"

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_warp_front_panel import BrickletWARPFrontPanel
import sys

from PIL import Image
import time

pixel_values_565 = []
pixel_values_565_8bit = []
for i in range(1):
    im = Image.open('warp_seq4/{0}.png'.format(i), 'r')
    width, height = im.size
    pixel_values = list(im.getdata())
    for value in pixel_values:
        try:
            r = value[0] >> 3
            g = value[1] >> 2
            b = value[2] >> 3
        except:
            r = value >> 3
            g = value >> 2
            b = value >> 3
        rgb565 = (r<<11) | (g<<5) | b
        pixel_values_565.append(rgb565)
        pixel_values_565_8bit.append(rgb565 & 0xFF)
        pixel_values_565_8bit.append(rgb565 >> 8)

def chunker(seq, size):
    return (seq[pos:pos + size] for pos in range(0, len(seq), size))

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    wfp = BrickletWARPFrontPanel(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd

    index = 0
    sub_index = 0
    print("Erase flash: ", wfp.erase_flash())
    time.sleep(1)
    wfp.set_flash_index(0, 0)
    for index, data in enumerate(chunker(pixel_values_565_8bit, 64)):
        if len(data) < 64:
            data = data + [0]*(64-len(data))
        ret = wfp.set_flash_data(data)
        while ret.status != BrickletWARPFrontPanel.FLASH_STATUS_OK:
            print("try again ", index, ret)
            ret = wfp.set_flash_data(data)
            time.sleep(0.002)
        else:
            print("ok ", index, ret)

#        print(index, ret)
