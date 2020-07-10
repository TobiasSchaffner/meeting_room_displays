# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
set(QEMU_EXTRA_FLAGS -s)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(meeting_room_display)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/mesh.c)

if(CONFIG_USB_CDC_ACM)
  target_sources(app PRIVATE src/serial.c)
else()
  target_sources(app PRIVATE src/gpio.c)
endif()

if(CONFIG_DISPLAY)
  target_sources(app PRIVATE src/display.c)
  target_sources(app PRIVATE src/power.c)
endif()

if(MESH_NODE_ADDR)
  zephyr_compile_definitions(MESH_NODE_ADDR=${MESH_NODE_ADDR})
endif()
