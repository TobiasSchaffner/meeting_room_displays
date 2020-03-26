/* main.c - Application main entry point */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <kernel.h>

#include <sys/printk.h>
#include <stdio.h>

#include "mesh.h"
#include "gpio.h"
#include "display.h"

static K_SEM_DEFINE(semaphore, 0, 1);

void on_button_1_press(void)
{
	mesh_send_button_message();
}

void on_button_2_press(void)
{
	u16_t target = mesh_set_target_address();

	if (target > 0x0009) {
		printk("A");
	} else {
		printk("%x", (target & 0xf));
	}
}

void on_button_3_press(void)
{
	const char *name = "1234567890123456789012345678901234567890\n";
	mesh_send_message(name, 42);
}

void on_button_4_press(void) { }

void on_message_received(char* message, u16_t len) {
	printk("Status: %s\n", message);
	display_set_status_message(message);
}

void on_heartbeat(u16_t hops) {
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
		printk("Address: 0x%04x\n", addr);
		display_set_status_address(addr);

		printk("Status: Initialized\n");
		display_set_status_message("Initialized");
	}

	while (1) {
		k_sem_take(&semaphore, K_FOREVER);
	}
}
