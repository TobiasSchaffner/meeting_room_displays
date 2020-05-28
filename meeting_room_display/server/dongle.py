import serial
import struct
import datetime

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
            if len(payload) > 255:
                raise ValueError("Payload must be smaller than 256 bytes.")
            message += struct.pack("B", len(payload))
            message += payload
        self._send(message)

    def send_appointment(self, address: int, start: datetime, end: datetime, description: str):
        start_time = float(start.hour) + start.minute / 60
        end_time = float(end.hour) + end.minute / 60
        payload = struct.pack("ffs", start_time, end_time, description.encode())
        self.send(address, 0x02, payload)