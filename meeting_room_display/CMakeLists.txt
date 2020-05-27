# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
set(QEMU_EXTRA_FLAGS -s)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(mesh_demo)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/mesh.c)

if(BOARD_NRF52840_PCA10056)
  target_sources(app PRIVATE src/gpio.c)
endif()

if(CONFIG_DISPLAY)
  target_sources(app PRIVATE src/display.c)
endif()

if(MESH_NODE_ADDR)
  zephyr_compile_definitions(MESH_NODE_ADDR=${MESH_NODE_ADDR})
endif()
