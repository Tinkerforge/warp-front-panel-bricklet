#!/usr/bin/env python3
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "2ehida" 

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_warp_front_panel import BrickletWARPFrontPanel

from PIL import Image
import time

im = Image.open('doom.png', 'r')
width, height = im.size
pixel_values = list(im.getdata())
pixel_values_565 = []
pixel_values_565_8bit = []
for value in pixel_values:
    r = value[0] >> 3
    g = value[1] >> 2
    b = value[2] >> 3
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
    wfp.set_flash_index(0, 0)
    for index, data in enumerate(chunker(pixel_values_565_8bit, 64)):
        ret = wfp.set_flash_data(data)
        while ret.status != BrickletWARPFrontPanel.FLASH_STATUS_OK:
            print("try again ", index, ret)
            ret = wfp.set_flash_data(data)

#        print(index, ret)