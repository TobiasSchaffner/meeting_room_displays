import cal
import dongle

import config

calendar = cal.Calendar()
dongle = dongle.Dongle()

for address, room in config.displays.items():
    print(f"Room {room}")
    for event in calendar.get_events(room):
        dongle.send_appointment(address, event.start, event.end, event.subject)