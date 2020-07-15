from message import MessageType

class Display:

    _date = "Mo, 01.01.01"
    _room = "0.000"

    _events = [None] * 16

    def __init__(self, dongle, address):
        self._dongle = dongle
        self._address = address

    def set_room(self, room: str):
        if self._room != room:
            print(f"Setting room {room} on {self._address}")
            self._dongle.send(self._address, MessageType.ROOM, room.encode("ASCII") + b"\x00")
            self._room = room

    def set_date(self, date: str):
        if self._date != date:
            print(f"Setting date {date} on {self._room}")
            self._dongle.send(self._address, MessageType.DATE, date.encode("ASCII")  + b"\x00")
            self._date = date

    def _free_slot(self):
        for index in range(len(self._events)):
            if self._events[index] is None:
                return index
        raise IndexError("Out of slots")

    def clear_events(self):
        print(f"Clearing events on {self._room}")
        self._dongle.send(self._address, MessageType.APPOINTMENTS_CLEAR)
        self._events = [None] * 10

    def set_events(self, events):
        for event in events:
            if event in [x for x in self._events if x is not None]:
                slot = self._events.index(event)
                saved_event = self._events[slot]
                if event.start != saved_event.start or event.end != saved_event.end or event.subject != saved_event.subject:
                    print(f"Updating event {event.subject} on {self._room}")
                    self._dongle.send(self._address, MessageType.APPOINTMENT_CLEAR, (slot).to_bytes(4, "little"))
                    self._dongle.send_appointment(self._address, slot, event.start, event.end, event.subject)
            else:
                slot = self._free_slot()
                print(f"Adding event {event.subject} on {self._room}")
                self._dongle.send_appointment(self._address, slot, event.start, event.end, event.subject)
                self._events[slot] = event
        for event in self._events:
            if event is not None and event not in events:
                slot = self._events.index(event)
                print(f"Removing event {event.subject} on {self._room}")
                self._dongle.send(self._address, MessageType.APPOINTMENT_CLEAR, (slot).to_bytes(4, "little"))
                self._events[slot] = None



            