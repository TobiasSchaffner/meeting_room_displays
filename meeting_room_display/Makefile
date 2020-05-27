all: build-all flash-all

build-all: build-display build-node build-sender

flash-all: flash-display flash-node flash-sender

display: build-display flash-display

build-display:
	west build -b nrf52840_pca10056 -d display-build -p auto . -- \
		-DSHIELD=waveshare_epaper_gdew075t7 \
		-DOVERLAY_CONFIG="display.conf debug.conf" \
		-DMESH_NODE_ADDR=0x030c

flash-display:
	@echo ">>>>>>>>DISPLAY<<<<<<<<"
	west flash -d display-build

node: build-node flash-node

build-node:
	west build -b nrf52840_pca10056 -d node-build -p auto . -- -DMESH_NODE_ADDR=0x030d

flash-node:
	@echo ">>>>>>>>NODE<<<<<<<<"
	west flash -d node-build

sender: build-sender flash-sender

build-sender:
	@echo ">>>>>>>>SENDER<<<<<<<<"
	west build -b nrf52840_pca10056 -d sender-build -p auto . -- -DMESH_NODE_ADDR=0x000f

flash-sender:
	west flash -d sender-build

dongle: build-dongle flash-dongle

build-dongle:
	@echo ">>>>>>>>SENDER<<<<<<<<"
	export SHIELD=""
	export BOARD=nrf52840_pca10059
	west build -b nrf52840_pca10059 -d dongle-build -p auto . -- -DSHIELD="" -DMESH_NODE_ADDR=0x000f
	nrfutil pkg generate --hw-version 52 --sd-req=0x00 \
        --application dongle-build/zephyr/zephyr.hex \
        --application-version 1 dongle-build/dongle.zip

flash-dongle:
	nrfutil dfu usb-serial -pkg dongle.zip -p /dev/ttyACM1
