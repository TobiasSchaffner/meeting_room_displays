import time
import locale
import datetime
import time

from message import MessageType
import config
from display import Display

locale.setlocale(locale.LC_TIME, 'de_DE.UTF-8')

class DisplayCluster(dict):

    def __init__(self, dongle):
        super().__init__()
        self._dongle = dongle
        self._dongle.send(config.group_addr, MessageType.SYNC)
        for address, room in config.displays.items():
            self[room] = Display(dongle, address)
            self[room].set_room(room)
            self[room].clear_events()

    def rooms(self):
        return self.keys()

    def set_date(self):
        date = datetime.datetime.now()
        date_string = date.strftime("%a, %d.%m.%y")
        for room in self.keys():
            self[room].set_date(date_string)

    def suspend(self):
        print(f"Sending displays to sleep for {config.interval * 60} seconds...")
        self._dongle.send(config.group_addr, MessageType.SUSPEND, int(config.interval * 60).to_bytes(4, "little"), ack_timeout=None)
        time.sleep(0.1)
        self._dongle.send(config.group_addr, MessageType.SYNC)
