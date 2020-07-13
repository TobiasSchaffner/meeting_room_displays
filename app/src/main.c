/* main.c - Application main entry point */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <logging/log.h>

#include "mesh.h"
#include "gpio.h"
#include "serial.h"
#include "display.h"
#include "message.h"
#include "power.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static K_SEM_DEFINE(semaphore, 0, 1);

void on_button_press(uint32_t button)
{
	switch(button) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			mesh_message_send(MESSAGE_APPOINTMENTS_CLEAR, MESH_GROUP_ADDR, NULL, 0);
			break;
		default:
			LOG_ERR("Button not supported.");
	}
}

void on_mesh_message_received(uint32_t message_type, uint16_t src_address, const void* payload, uint16_t len)
{
	if (message_type == MESSAGE_OK)
		serial_message_send(message_type, src_address, payload, len);
	else {
		mesh_message_send(MESSAGE_OK, src_address, NULL, 0);
		switch(message_type) {
			case MESSAGE_DAY:
				display_set_date((char*) payload);
				break;
			case MESSAGE_ROOM:
				display_set_room((char*) payload);
				break;
			case MESSAGE_APPOINTMENT:
				display_create_appointment((message_appointment*) payload);
				break;
			case MESSAGE_APPOINTMENT_CLEAR:
				display_clear_appointment(*((int*) payload));
				break;
			case MESSAGE_APPOINTMENTS_CLEAR:
				display_clear_appointments();
				break;
			case MESSAGE_SUSPEND:
				power_suspend(*((int*) payload));
				break;
			case MESSAGE_BUTTON:
				LOG_INF("Received Button Press.");
				break;
			case MESSAGE_STRING:
				LOG_INF("Received String: %s", (char*) payload);
				break;
			default:
				LOG_ERR("Message type not supported.");
		}
	}
}

void on_serial_message_received(uint32_t message_type, uint16_t dst_address, const void* payload, uint16_t len) {
	mesh_message_send(message_type, dst_address, payload, len);
}

void on_mesh_heartbeat(uint16_t hops)
{
	LOG_INF("Hops: %d\n", hops);
	display_set_status_heartbeat(hops);
}

void on_power_suspend(void)
{
	LOG_INF("Suspending...");
	display_set_status_message("Suspended");
	mesh_suspend();
}

void on_power_resume(void)
{
	mesh_resume();
	display_set_status_message("Listening");
	LOG_INF("Resumed");
}

void main(void)
{
	int err = 0;

	LOG_INF("Status: Initializing\n");

	if (!err)
		err = serial_init();

	if (!err)
		err = display_init();

	if (!err)
		err = gpio_init();

	if (!err)
		err = mesh_init();

	if (err) {
		LOG_ERR("Status: Error\n");
		display_set_status_message("Error");
		k_sem_take(&semaphore, K_FOREVER);
	}
	else {
		LOG_INF("Address: 0x%04x", MESH_NODE_ADDR);
		display_set_status_address(MESH_NODE_ADDR);
		LOG_INF("Status: Initialized");
		display_set_status_message("Initialized");
	}

	if (!err)
		err = power_init();

	while (1) {
		k_sem_take(&semaphore, K_FOREVER);
	}
}
