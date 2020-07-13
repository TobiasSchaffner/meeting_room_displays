# Meeting room display server application

The server application for the meeting room display cluster.

It fetches the rooms appointments of the day from a specified office 365
calendar and sends the information to the configured displays.

## Setup

* Go to app/ folder and create a dongle with make dongle
* Make sure that the attached dongle is flashable by pressing the red button
* Remove the dongle when flash is finished
* Change back to server/ folder
* Copy the config.py.template to config.py and enter your credentials
* You find detailed information on the credentials on https://pypi.org/project/O365/
* Start the server application with pyhton3.5 server.py
