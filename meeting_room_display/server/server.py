import cal
import dongle
import datetime
import locale
import time

import config
from message import MessageType

locale.setlocale(locale.LC_TIME, 'de_DE.UTF-8')
date = datetime.datetime.now()
date_string = date.strftime("%A, %d.%m.%y")
print(date_string)

calendar = cal.Calendar()

try:
    dongle = dongle.Dongle()

    for address, room in config.displays.items():
        dongle.send(address, MessageType.DATE, date_string.encode("ASCII")  + b"\x00")
        time.sleep(10)
        dongle.send(address, MessageType.ROOM, room.encode("ASCII") + b"\x00")
        print(f"Room {room}, Address: {address}")
        for event in calendar.get_events(room):
            print(event)
            dongle.send_appointment(address, event.start, event.end, event.subject)

except KeyboardInterrupt:
    pass
finally:
    dongle.disconnect()
