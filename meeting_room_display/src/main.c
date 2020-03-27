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
			mesh_send_message(MESH_MESSAGE_BUTTON, NULL, 0);
			break;
		case 2:
			mesh_increment_target_address();

			if (mesh_target > 0x0009) {
				printk("A");
			} else {
				printk("%x", (mesh_target & 0xf));
			}
			break;
		case 3:
			mesh_send_message(MESH_MESSAGE_STRING, test_string, 13);
			break;
		case 4:
		default:
			break;
	}
}

void on_message_received(u32_t message_type, const void* message, u16_t len)
{
	printk("Status: %s\n", (char*) message);
	display_set_status_message(message);
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
		printk("Address: 0x%04x\n", mesh_addr);
		display_set_status_address(mesh_addr);

		printk("Status: Initialized\n");
		display_set_status_message("Initialized");
	}

	while (1) {
		k_sem_take(&semaphore, K_FOREVER);
	}
}
