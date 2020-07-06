import struct
import datetime
from time import sleep
from queue import Queue, Empty

import config
from message import MESSAGE_SERIAL_BEGIN, MESSAGE_HEADER_LEN, MessageType
from tty import TTY


class Dongle():

    def __init__(self):
        self._queue = Queue()
        self._serial = TTY(self._queue)
        self._serial.connect()
        self._serial.start()

    def disconnect(self):
        self._serial.stop()
        self._serial.join()

    def send(self, address: int, message_type: MessageType, payload: bytes=b''):
        print(f"Sending: Address {address} Type: {message_type} Payload: {payload}")
        message = struct.pack("3sBHH", MESSAGE_SERIAL_BEGIN, MESSAGE_HEADER_LEN + len(payload), address, message_type.value)
        if payload:
            if len(payload) > 255 - 7:
                raise ValueError(f"Payload must be smaller than {256 - 7} bytes.")
            message += payload
        self._serial.send(message)

    def receive(self):
        while(True):
            try:
                return self._queue.get_nowait()
            except Empty:
                pass
            sleep(0.1)

    def send_appointment(self, address: int, start: datetime, end: datetime, description: str):
        start_time = float(start.hour) + start.minute / 60
        end_time = float(end.hour) + end.minute / 60
        payload = struct.pack("ff", start_time, end_time)
        payload += description.encode()
        self.send(address, MessageType.APPOINTMENT, payload)
        self.receive()
