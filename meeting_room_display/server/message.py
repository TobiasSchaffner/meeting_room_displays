from enum import Enum

MESSAGE_SERIAL_BEGIN = b'###'
MESSAGE_MIN_LEN = 8
MESSAGE_HEADER_LEN = MESSAGE_MIN_LEN - len(MESSAGE_SERIAL_BEGIN)


class MessageType(Enum):

    OK = 0x00
    DATE = 0x01
    ROOM = 0x02
    APPOINTMENT = 0x03
    APPOINTMENTS_CLEAR = 0x04
    SUSPEND = 0x05

    STRING = 0x0e
    BUTTON = 0x0f


class Message:

    def __init__(self, length, address, method_type):
        self.length = length
        self.address = address
        self.method_type = method_type
        self.payload = b''