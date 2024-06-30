#!/usr/bin/env python3
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "2ehida" 

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_warp_front_panel import BrickletWARPFrontPanel

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    wfp = BrickletWARPFrontPanel(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    print(wfp.get_eeprom_index())
