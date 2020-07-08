/* main.c - Application main entry point */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <kernel.h>
#include <sys/printk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mesh.h"
#include "gpio.h"
#include "serial.h"
#include "display.h"
#include "message.h"
#include "power.h"

#include <power/power.h>


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
			printk("Button not supported.\n");
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
			case MESSAGE_APPOINTMENTS_CLEAR:
				display_clear_appointments();
				break;
			case MESSAGE_SUSPEND:
				power_suspend(*((uint32_t*) payload));
				break;
			case MESSAGE_BUTTON:
				printk("Received Button Press.\n");
				break;
			case MESSAGE_STRING:
				printk("Received String: %s\n", (char*) payload);
				break;
			default:
				printk("Message type not supported.\n");
		}
	}
}

void on_serial_message_received(uint32_t message_type, uint16_t dst_address, const void* payload, uint16_t len) {
	mesh_message_send(message_type, dst_address, payload, len);
}

void on_mesh_heartbeat(uint16_t hops)
{
	printk("Hops: %d\n", hops);
	display_set_status_heartbeat(hops);
}

void main(void)
{
	int err = 0;

	printk("Status: Initializing\n");

	if (!err)
		err = serial_init();

	if (!err)
		err = display_init();

	if (!err)
		err = gpio_init();

	if (!err)
		err = mesh_init();

	if (err) {
		printk("Status: Error\n");
		display_set_status_message("Error");
		k_sem_take(&semaphore, K_FOREVER);
	}
	else {
		printk("Address: 0x%04x\n", MESH_NODE_ADDR);
		display_set_status_address(MESH_NODE_ADDR);
		printk("Status: Initialized\n");
		display_set_status_message("Initialized");
	}

	if (!err)
		err = power_init();

	while (1) {
		k_sem_take(&semaphore, K_FOREVER);
	}
}
