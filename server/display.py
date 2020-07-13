from message import MessageType

class Display:

    _date = "Mo, 01.01.01"
    _room = "0.000"

    _events = [None] * 10

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

    def _free_slot(self):
        for index in range(len(self._events)):
            if self._events[index] is None:
                return index
        raise IndexError("Out of slots")

    def clear_events(self):
        self._dongle.send(self._address, MessageType.APPOINTMENTS_CLEAR)
        self._events = [None] * 10

    def set_events(self, events):
        for event in events:
            if not event in [x for x in self._events if x is not None]:
                slot = self._free_slot()
                self._dongle.send_appointment(self._address, slot, event.start, event.end, event.subject)
                self._events[slot] = event
        for event in self._events:
            if event is not None and event not in events:
                slot = self._events.index(event)
                self._dongle.send(self.address, MessageType.APPOINTMENT_CLEAR, (slot).to_bytes(4, "little"))
                self._events[slot] = None



            