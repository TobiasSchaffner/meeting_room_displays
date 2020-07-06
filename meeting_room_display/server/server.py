import cal
import dongle

import config

calendar = cal.Calendar()

try:
    dongle = dongle.Dongle()

    for address, room in config.displays.items():
        print(f"Room {room}, Address: {address}")
        for event in calendar.get_events(room):
            print(event)
            dongle.send_appointment(address, event.start, event.end, event.subject + "\x00")

except KeyboardInterrupt:
    pass
finally:
    dongle.disconnect()
