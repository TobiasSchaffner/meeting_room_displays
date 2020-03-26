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

static char test_string[] = "A502 .5§Freitag 25.06.20§Wolram: Trumpf§9.0§9.5§Tobias: Espe§12.0§13§Benedikt: Linux§16.0§17.0";

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

void on_message_received(char* message, u16_t len)
{
	printk("Status: %s\n", message);
	display_set_status_message(message);
}

void on_heartbeat(u16_t hops)
{
	printk("Hops: %d\n", hops);
	display_set_status_heartbeat(hops);
}

static void parse_message(char* message)
{
	const char delimiter[2] = "§";

	printk("Parsing\n");
	char* room = strtok(message, delimiter);
	if (room == NULL) return;
	printk("%s\n", room);

	char* date = strtok(NULL, delimiter);
	if (date == NULL) return;
	printk("%s\n", date);

	char* element_name;
	float start;
	float end;
	while ((element_name = strtok(NULL, delimiter)) != NULL) {
		start = atof(strtok(NULL, delimiter));
		end = atof(strtok(NULL, delimiter));
		display_create_appointment(element_name, start, end);
	}
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

	parse_message(test_string);
	display_clear_appointments();

	while (1) {
		k_sem_take(&semaphore, K_FOREVER);
	}
}
