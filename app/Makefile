DISPLAY_ADDRESS=0x030c

all: build-all flash-all

build-all: build-display build-relay build-dongle

flash-all: flash-display flash-relay flash-dongle

clean:
	rm -rf display-build
	rm -rf relay-build
	rm -rf dongle-build

display: build-display flash-display

build-display:
	west build -b nrf52840dk_nrf52840 -d display-build -p auto . -- \
		-DSHIELD=waveshare_epaper_gdew075t7 \
		-DOVERLAY_CONFIG="display.conf" \
		-DMESH_NODE_ADDR=${DISPLAY_ADDRESS}

flash-display:
	@echo ">>>>>>>>DISPLAY<<<<<<<<"
	west flash -d display-build

relay: build-relay flash-relay

build-relay:
	west build -b nrf52840dk_nrf52840 -d relay-build -p auto . -- \
		-DOVERLAY_CONFIG="debug.conf" \
		-DMESH_NODE_ADDR=0x030d

flash-relay:
	@echo ">>>>>>>>NODE<<<<<<<<"
	west flash -d node-build

dongle: build-dongle flash-dongle

build-dongle:
	@echo ">>>>>>>>DONGLE<<<<<<<<"
	west build -b nrf52840dongle_nrf52840 -d dongle-build -p auto . -- \
		-DOVERLAY_CONFIG="serial.conf" \
		-DMESH_NODE_ADDR=0x000f
	nrfutil pkg generate --hw-version 52 --sd-req=0x00 \
        --application dongle-build/zephyr/zephyr.hex \
        --application-version 1 dongle-build/dongle.zip

flash-dongle:
	nrfutil dfu usb-serial -pkg dongle-build/dongle.zip -p /dev/ttyDisplayDongle
