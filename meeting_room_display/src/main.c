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

static K_SEM_DEFINE(semaphore, 0, 1);

static void send_appointment(u16_t address, float start, float end, const char* description)
{
	u32_t buf_size = sizeof(message_appointment) + strlen(description);
	u8_t buf[buf_size];

	message_appointment* appointment = (message_appointment*) &buf;
	appointment->start = start;
	appointment->end = end;
	strcpy(&buf[sizeof(message_appointment)], description);

	mesh_send_message(MESH_MESSAGE_APPOINTMENT, MESH_GROUP_ADDR, appointment, buf_size);
}

void on_button_press(u32_t button)
{
	switch(button) {
		case 1:
			send_appointment(MESH_GROUP_ADDR, 9.75, 10.5, "Hase schlafert noch mal");
			break;
		case 2:
			send_appointment(MESH_GROUP_ADDR, 13.00, 14.50, "Mausbaeren schnuckeln");
			break;
		case 3:
			send_appointment(MESH_GROUP_ADDR, 16.25, 17.75, "Nach der Arbeit schnuckel");
			break;
		case 4:
			mesh_send_message(MESH_MESSAGE_APPOINTMENTS_CLEAR, MESH_GROUP_ADDR, NULL, 0);
			break;
		default:
			printk("Button not supported.\n");
	}
}

void on_message_received(u32_t message_type, u16_t address, const void* message, u16_t len)
{
	printk("Received message: Type: %d Address: 0x%04x Len: %d Content: %s\n", message_type, address, len, (char*) message);

	switch(message_type) {
		case MESH_MESSAGE_BUTTON:
			printk("Received Button Press.\n");
			break;
		case MESH_MESSAGE_STRING:
			printk("Received String: %s\n", (char*) message);
			break;
		case MESH_MESSAGE_APPOINTMENT:
			display_create_appointment((message_appointment*) message);
			break;
		case MESH_MESSAGE_APPOINTMENTS_CLEAR:
			display_clear_appointments();
			break;
		default:
			printk("Message type not supported.\n");
	}
}

void on_serial_message_received(const void* message, u16_t len) {
	printk("%s", message);
}

void on_heartbeat(u16_t hops)
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
	}

	if (!err) {
		printk("Address: 0x%04x\n", MESH_NODE_ADDR);
		display_set_status_address(MESH_NODE_ADDR);

		printk("Status: Initialized\n");
		display_set_status_message("Initialized");
	}

	while (1) {
		k_sem_take(&semaphore, K_FOREVER);
	}
}
