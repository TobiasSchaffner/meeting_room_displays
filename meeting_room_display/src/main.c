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
#include "display.h"

static K_SEM_DEFINE(semaphore, 0, 1);

void on_button_press(u32_t button)
{
	const char test_string[] = "Wolfram: Esp";

	switch(button) {
		case 1:
			mesh_send_message(MESH_MESSAGE_BUTTON, MESH_GROUP_ADDR, NULL, 0);
			break;
		case 2:
			break;
		case 3:
			mesh_send_message(MESH_MESSAGE_STRING, MESH_GROUP_ADDR, test_string, 13);
			break;
		case 4:
			break;
		default:
			printk("Button not supported.\n");
	}
}

void on_message_received(u32_t message_type, u16_t address, const void* message, u16_t len)
{
	printk("Received message: Type: %d Address: 0x%04x Content: %s\n", message_type, address, (char*) message);

	switch(message_type) {
		case MESH_MESSAGE_BUTTON:
			printk("Received Button Press.\n");
			break;
		case MESH_MESSAGE_STRING:
			printk("Received String: %s\n", (char*) message);
			break;
		default:
			printk("Message type not supported.\n");
	}
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
	
	err = display_init();	

	if (!err) {
		err = gpio_init();

		if (!err)
			err = mesh_init();

		if (err) {
			printk("Status: Error\n");
			display_set_status_message("Error");
		}
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
