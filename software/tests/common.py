#!/usr/bin/env python3
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
found_uid = None

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_warp_front_panel import BrickletWARPFrontPanel

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
