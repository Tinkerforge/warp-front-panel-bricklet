#!/usr/bin/env python3
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
found_uid = None

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_warp_front_panel import BrickletWARPFrontPanel
import sys

import time

def cb_enumerate(uid, connected_uid, position, hardware_version, firmware_version, device_identifier, enumeration_type):
    global found_uid

    if device_identifier == BrickletWARPFrontPanel.DEVICE_IDENTIFIER:
        found_uid = uid

def get_first_warp_front_panel_bricklet():
    global found_uid

    ipcon = IPConnection()
    ipcon.connect(HOST, PORT)
    ipcon.register_callback(IPConnection.CALLBACK_ENUMERATE, cb_enumerate)
    ipcon.enumerate()
    for i in range(100):
        if found_uid != None:
            return BrickletWARPFrontPanel(found_uid, ipcon)
        time.sleep(0.01)

    return None

def chunker(seq, size):
    return (seq[pos:pos + size] for pos in range(0, len(seq), size))

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    wfp = get_first_warp_front_panel_bricklet()

    ipcon.connect(HOST, PORT) # Connect to brickd

    with open('flash_map.bin', 'rb') as f:
        index = 0
        sub_index = 0
        print("Erase flash: ", wfp.erase_flash())
        time.sleep(1)
        wfp.set_flash_index(0, 0)
        for index, data in enumerate(chunker(f.read(), 64)):
            if len(data) < 64:
                data = data + [0]*(64-len(data))
            ret = wfp.set_flash_data(data)
            while ret.status != BrickletWARPFrontPanel.FLASH_STATUS_OK:
                print("try again ", index, ret)
                ret = wfp.set_flash_data(data)
                time.sleep(0.002)
            else:
                print("ok ", index, ret)
