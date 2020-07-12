from message import MessageType

class Display:

    _date = "Mo, 01.01.01"
    _room = "0.000"

    _events = []

    def __init__(self, dongle, address):
        self._dongle = dongle
        self._address = address

    def set_room(self, room: str):
        if self._room != room:
            self._dongle.send(self._address, MessageType.ROOM, room.encode("ASCII") + b"\x00")
            self._room = room

    def set_date(self, date: str):
        if self._date != date:
            self._dongle.send(self._address, MessageType.DATE, date.encode("ASCII")  + b"\x00")
            self._date = date

    def set_events(self, events):
        if events != self._events:
            self._dongle.send(self._address, MessageType.APPOINTMENTS_CLEAR)
            for event in events:
                self._dongle.send_appointment(self._address, event.start, event.end, event.subject)
            self._events = events


            