import serial
import struct
from threading import Thread
from queue import Queue
from time import sleep, time


from message import MESSAGE_SERIAL_BEGIN, MESSAGE_HEADER_LEN, MESSAGE_MIN_LEN, Message
import config

class TTY(Thread):

    _ser = None

    def __init__(self, message_queue):
        print("Waiting for USB Dongle...")
        while not self._ser:
            try:
                self._ser = serial.Serial(config.dongle_port, 115200)
            except:
                sleep(0.01)
        Thread.__init__(self)
        self._queue = message_queue
        self._die = False

    def stop(self):
        self._die = True

    def send(self, message: bytes):
        print(f"Sending Bytes: {message}")
        self._ser.write(message)

    def connect(self):
        start = time()
        print("Initializing")
        while time() < start + 15:
            if (self._ser.inWaiting() > 0):
                print(self._ser.read_all().decode(), sep="")
            sleep(0.1)

    def run(self):
        buf = b''
        marker_found = False
        message = None

        # Run
        while not self._die:
            if (self._ser.inWaiting() > 0):
                buf += self._ser.read_all()
                if not marker_found:
                    start = buf.find(MESSAGE_SERIAL_BEGIN)
                    if (start == -1):
                        print(buf.decode(), end='')
                        buf = b''
                    else:
                        print(buf[:start].decode(), end='')
                        buf = buf[start:]
                        marker_found = True
                if marker_found and len(buf) >= MESSAGE_MIN_LEN:
                    _, length, address, method_type = struct.unpack("3sBHH", buf[:MESSAGE_MIN_LEN])
                    message = Message(length, address, method_type)
                if message is not None and len(buf) >= message.length:
                    print(f"Received Bytes: {buf}")
                    message.payload = buf[MESSAGE_MIN_LEN:]
                    self._queue.put(message)
                    message = None
                    marker_found = False
                    buf = b''
            sleep(0.1)