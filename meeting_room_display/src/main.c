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

static char addr_message[40] = {0};
static char center_message[40] = {0};
static char hop_message[20] = {0};

static K_SEM_DEFINE(semaphore, 0, 1);

static void display(char *str)
{
	sprintf(center_message, "%s", str);
	k_sem_give(&semaphore);
}

void on_button_1_press(void)
{
	send_button_message();
}

void on_button_2_press(void)
{
	u16_t target = set_target();

	if (target > 0x0009) {
		printk("A");
	} else {
		printk("%x", (target & 0xf));
	}
}

void on_button_3_press(void)
{
	const char *name = "1234567890123456789012345678901234567890\n";
	send_message(name, 42);
}

void on_button_4_press(void) { }

void on_message_received(char* message, u16_t len) {
	printk("Message is ");
	for (int i=0; i < len; i++)
		printk("%x", message[i]);
	printk("\n");
	display(message);
}

void on_heartbeat(u16_t hops) {
	printk("Heartbeat arrived with %u hops.\n", hops);
	sprintf(hop_message, "Hops: %d", hops);
	display_set_bottom_label(hop_message);
}

void main(void)
{
	printk("Initializing...\n");
		
	gpio_init();
	display_init();
	mesh_init();

	sprintf(addr_message, "Address: 0x%04x", addr);
	display_set_center_label(addr_message);
	display_set_top_label("Status: Initialized");

	while (1) {
		k_sem_take(&semaphore, K_FOREVER);
		display_set_center_label(center_message);
		printk("%s", center_message);
	}
}
