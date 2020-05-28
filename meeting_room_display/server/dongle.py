import serial
import struct

import config

class Dongle():

    def __init__(self):
        #self._ser = serial.Serial(config.dongle_port)
        pass

    def _send(self, message: bytes):
        #self._ser.write(message)
        print(message)

    def send(self, address: int, type: int, payload: bytes=None):
        message = b'##'
        message += struct.pack("HH", address, type)
        if payload:
            message += payload
        self._send(message)