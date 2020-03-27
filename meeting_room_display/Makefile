all: build-all flash-all

build-all: build-display build-node build-sender

flash-all: flash-display flash-node flash-sender

display: build-display flash-display

build-display:
	west build -d display-build -p auto . -- -DOVERLAY_CONFIG="display.conf debug.conf" -DMESH_NODE_ADDR=0x030c

flash-display:
	@echo ">>>>>>>>DISPLAY<<<<<<<<"
	west flash -d display-build

node: build-node flash-node

build-node:
	west build -d node-build -p auto . -- -DMESH_NODE_ADDR=0x030d

flash-node:
	@echo ">>>>>>>>NODE<<<<<<<<"
	west flash -d node-build

sender: build-sender flash-sender

build-sender:
	@echo ">>>>>>>>SENDER<<<<<<<<"
	west build -d sender-build -p auto . -- -DMESH_NODE_ADDR=0x000f

flash-sender:
	west flash -d sender-build
