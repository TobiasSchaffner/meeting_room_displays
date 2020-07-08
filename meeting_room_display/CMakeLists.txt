# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
set(QEMU_EXTRA_FLAGS -s)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(mesh_demo)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/mesh.c)
target_sources(app PRIVATE src/power.c)

if(CONFIG_USB_CDC_ACM)
  message("CONFIG_USB_CDC_ACM")
  target_sources(app PRIVATE src/serial.c)
else()
  message("GPIO")
  target_sources(app PRIVATE src/gpio.c)
endif()

if(CONFIG_DISPLAY)
  target_sources(app PRIVATE src/display.c)
endif()

if(MESH_NODE_ADDR)
  zephyr_compile_definitions(MESH_NODE_ADDR=${MESH_NODE_ADDR})
endif()
