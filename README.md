# Meeting room displays

Bluetooth mesh based long lasting meeting room displays.

This repository provides the code for the displays as well as the server
application that fetches the meeting room appointments from an office 365
calendar and sends the information to the configured displays. The server
application will use a usb dongle to send the data to the displays.

## Hardware
The dongle used for the server application is the Nordic nRF52840 Dongle:
https://www.nordicsemi.com/Software-and-tools/Development-Kits/nRF52840-Dongle

The displays use the Nordic nRF52840 DK:
https://www.nordicsemi.com/Software-and-Tools/Development-Kits/nRF52840-DK

and the waveshare 7.5 inch e-paper HAT:
https://www.waveshare.com/wiki/7.5inch_e-Paper_HAT

please note that at the moment only revision 1 of the e-Paper HAT is working.

## Checkout and init
The project is based on zephyr RTOS. To clone this repository as well as
everything needed call:
```bash
west init -m https://github.com/TobiasSchaffner/meeting_room_displays.git
west update
```

## Udev rule
To make it easier to flash the dongle it is recommended to apply the provided
Udev rule. It will create a symlink which will always have the same name no
matter which ttyACM enumeration it got.
```bash
sudo cp meeting_room_displays/20_DiplayDongleUDev.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules && sudo udevadm trigger
```

## Installing Zephyr SDK
To install the SDK follow the steps provided in the official
[Zephyr documentation](https://docs.zephyrproject.org/latest/getting_started/index.html).

## Installing nrfutil
In order to flash the Nordic dongle the nrfutil package is required.
Install it via pip:
```
pip install nrfutil
```

## Install nrfjprog
nrfjprog is required to flash/debug the nRF52840 development kit (in this case: the display). Download the tool 
[here](https://www.nordicsemi.com/Products/Development-tools/nrf-command-line-tools/download).
Debian based systems may install the `.deb` package via
```
sudo dpkg -i <nrf-debian-package-name>.deb
```

nrfjprog requires SEGGER's JLink Software in order to run. Download the appropriate installer for
your system [here](https://www.segger.com/downloads/jlink).
As in the previous step, Debian based systems may use the `.deb` and install it the same way as above.

## Adding the user to the dialout group
Make sure to add the user who is working on this project to the `dialout` group so
that she can access `/dev/<tty-device>` (eg. the dongle):
```
sudo usermod -a -G dialout <username>
```
Do not forget to start a new terminal afterwards.

## Build
To build one of the components use the provided make file in
meeting_room_displays/app/

E.g. to build and flash a display call:
```bash
make display
```

To build and flash the dongle:
Insert the dongle into a USB port and press the reset button. A red
LED will light up. Then call:
```bash
make dongle
```

