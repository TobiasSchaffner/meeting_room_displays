/* microbit.c - BBC micro:bit specific hooks */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <drivers/gpio.h>
#include <soc.h>
#include <ctype.h>
#include <bluetooth/mesh.h>

#include "gpio.h"

#include <logging/log.h>

LOG_MODULE_REGISTER(gpio, LOG_LEVEL_INF);

#define FLAGS_OR_ZERO(node)						\
	COND_CODE_1(DT_PHA_HAS_CELL(node, gpios, flags),		\
		    (DT_GPIO_FLAGS(node, gpios)),			\
		    (0))

#define SW0_NODE	DT_ALIAS(sw0)
#define SW0_GPIO_LABEL	DT_GPIO_LABEL(SW0_NODE, gpios)
#define SW0_GPIO_PIN	DT_GPIO_PIN(SW0_NODE, gpios)
#define SW0_GPIO_FLAGS	(GPIO_INPUT | FLAGS_OR_ZERO(SW0_NODE))

#define SW1_NODE	DT_ALIAS(sw1)
#define SW1_GPIO_LABEL	DT_GPIO_LABEL(SW1_NODE, gpios)
#define SW1_GPIO_PIN	DT_GPIO_PIN(SW1_NODE, gpios)
#define SW1_GPIO_FLAGS	(GPIO_INPUT | FLAGS_OR_ZERO(SW1_NODE))

#define SW2_NODE	DT_ALIAS(sw2)
#define SW2_GPIO_LABEL	DT_GPIO_LABEL(SW2_NODE, gpios)
#define SW2_GPIO_PIN	DT_GPIO_PIN(SW2_NODE, gpios)
#define SW2_GPIO_FLAGS	(GPIO_INPUT | FLAGS_OR_ZERO(SW2_NODE))

#define SW3_NODE	DT_ALIAS(sw3)
#define SW3_GPIO_LABEL	DT_GPIO_LABEL(SW3_NODE, gpios)
#define SW3_GPIO_PIN	DT_GPIO_PIN(SW3_NODE, gpios)
#define SW3_GPIO_FLAGS	(GPIO_INPUT | FLAGS_OR_ZERO(SW3_NODE))


static struct device *gpio;

static struct k_work button_1_work;
static struct k_work button_2_work;
static struct k_work button_3_work;
static struct k_work button_4_work;

static void button_1_pressed(struct k_work *work){
	on_button_press(1);}
static void button_2_pressed(struct k_work *work){
	on_button_press(2);}
static void button_3_pressed(struct k_work *work){
	on_button_press(3);}
static void button_4_pressed(struct k_work *work){
	on_button_press(4);}

static void button_pressed(struct device *dev, struct gpio_callback *cb,
			   uint32_t pins)
{
	if (pins & BIT(SW0_GPIO_PIN)) {
		k_work_submit(&button_1_work);}
	else if (pins & BIT(SW1_GPIO_PIN)) {
		k_work_submit(&button_2_work);}
	else if (pins & BIT(SW2_GPIO_PIN)) {
		k_work_submit(&button_3_work);}
	else if (pins & BIT(SW3_GPIO_PIN)) {
		k_work_submit(&button_4_work);}
	else {
		LOG_ERR("Unexpected button press.\n");}
}

static void configure_buttons(void)
{
	static struct gpio_callback button_cb;

	k_work_init(&button_1_work, button_1_pressed);
	k_work_init(&button_2_work, button_2_pressed);
	k_work_init(&button_3_work, button_3_pressed);
	k_work_init(&button_4_work, button_4_pressed);

	gpio = device_get_binding(SW0_GPIO_LABEL);

	gpio_pin_configure(gpio, SW0_GPIO_PIN,
			   GPIO_INPUT | SW0_GPIO_FLAGS);
	gpio_pin_interrupt_configure(gpio, SW0_GPIO_PIN,
				     GPIO_INT_EDGE_TO_ACTIVE);

	gpio_pin_configure(gpio, SW1_GPIO_PIN,
			   GPIO_INPUT | SW1_GPIO_FLAGS);
	gpio_pin_interrupt_configure(gpio, SW1_GPIO_PIN,
				     GPIO_INT_EDGE_TO_ACTIVE);

	gpio_pin_configure(gpio, SW2_GPIO_PIN,
			   GPIO_INPUT | SW2_GPIO_FLAGS);
	gpio_pin_interrupt_configure(gpio, SW2_GPIO_PIN,
				     GPIO_INT_EDGE_TO_ACTIVE);

	gpio_pin_configure(gpio, SW3_GPIO_PIN,
			   GPIO_INPUT | SW3_GPIO_FLAGS);
	gpio_pin_interrupt_configure(gpio, SW3_GPIO_PIN,
				     GPIO_INT_EDGE_TO_ACTIVE);

	gpio_init_callback(&button_cb, button_pressed,
			   BIT(SW0_GPIO_PIN) | BIT(SW1_GPIO_PIN) |
			   BIT(SW2_GPIO_PIN) | BIT(SW3_GPIO_PIN));
	gpio_add_callback(gpio, &button_cb);
}

int gpio_init()
{
	configure_buttons();
	return 0;
}
