/* microbit.c - BBC micro:bit specific hooks */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <drivers/gpio.h>
#include <soc.h>
#include <sys/printk.h>
#include <ctype.h>
#include <bluetooth/mesh.h>

#include "gpio.h"

static struct device *gpio;

static struct k_work button_1_work;
static struct k_work button_2_work;

static void button_1_pressed(struct k_work *work)
{
	on_button_1_press();
}

static void button_2_pressed(struct k_work *work)
{
	on_button_2_press();
}

static void button_pressed(struct device *dev, struct gpio_callback *cb,
			   u32_t pins)
{
	if (pins & BIT(DT_ALIAS_SW0_GPIOS_PIN)) {
		k_work_submit(&button_1_work);
	}
	else if (pins & BIT(DT_ALIAS_SW1_GPIOS_PIN)) {
		k_work_submit(&button_2_work);
	}
	else {
		printk("Unexpected button press.\n");
	}
}

static void configure_buttons(void)
{
	static struct gpio_callback button_cb;

	k_work_init(&button_1_work, button_1_pressed);
	k_work_init(&button_2_work, button_2_pressed);

	gpio = device_get_binding(DT_ALIAS_SW0_GPIOS_CONTROLLER);

	gpio_pin_configure(gpio, DT_ALIAS_SW0_GPIOS_PIN,
			   GPIO_INPUT | DT_ALIAS_SW0_GPIOS_FLAGS);
	gpio_pin_interrupt_configure(gpio, DT_ALIAS_SW0_GPIOS_PIN,
				     GPIO_INT_EDGE_TO_ACTIVE);

	gpio_pin_configure(gpio, DT_ALIAS_SW1_GPIOS_PIN,
			   GPIO_INPUT | DT_ALIAS_SW1_GPIOS_FLAGS);
	gpio_pin_interrupt_configure(gpio, DT_ALIAS_SW1_GPIOS_PIN,
				     GPIO_INT_EDGE_TO_ACTIVE);

	gpio_init_callback(&button_cb, button_pressed,
			   BIT(DT_ALIAS_SW0_GPIOS_PIN) | BIT(DT_ALIAS_SW1_GPIOS_PIN));
	gpio_add_callback(gpio, &button_cb);
}

void gpio_init()
{
	configure_buttons();
}
