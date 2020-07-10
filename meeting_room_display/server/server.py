import cal
import dongle
import datetime
import locale
import time

import config
from message import MessageType

locale.setlocale(locale.LC_TIME, 'de_DE.UTF-8')

calendar = cal.Calendar()

try:
    dongle = dongle.Dongle()
    while True:
        date = datetime.datetime.now()
        date_string = date.strftime("%a, %d.%m.%y")

        for address, room in config.displays.items():
            print(f"Room {room}, Address: {address}")
            dongle.send(address, MessageType.DATE, date_string.encode("ASCII")  + b"\x00")
            dongle.send(address, MessageType.ROOM, room.encode("ASCII") + b"\x00")
            for event in calendar.get_events(room):
                print(event)
                dongle.send_appointment(address, event.start, event.end, event.subject)
        dongle.send(config.group_addr, MessageType.SUSPEND, int(config.interval * 60).to_bytes(4, "little"))
        print(f"Going to sleep for {config.interval * 60 + 5} seconds...")
        time.sleep(config.interval * 60 + 5)

except KeyboardInterrupt:
    pass
finally:
    dongle.disconnect()
