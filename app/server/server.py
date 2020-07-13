from o365_calendar import Calendar
from dongle import Dongle
from display_cluster import DisplayCluster

calendar = Calendar()
dongle = Dongle()

try:
    displays = DisplayCluster(dongle)

    while True:
        displays.set_date()

        for room in displays.rooms():
            displays[room].set_events(calendar.get_events(room))
        
        displays.suspend()

except KeyboardInterrupt:
    pass
finally:
    dongle.disconnect()
