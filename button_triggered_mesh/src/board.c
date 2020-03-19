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
#include <drivers/gpio.h>
#include <drivers/pwm.h>

#include <bluetooth/mesh.h>

#include "board.h"

#define SCROLL_SPEED   K_MSEC(300)

#define BUZZER_PIN     EXT_P0_GPIO_PIN
#define BEEP_DURATION  K_MSEC(60)

#define SEQ_PER_BIT  976
#define SEQ_PAGE     (NRF_FICR->CODEPAGESIZE * (NRF_FICR->CODESIZE - 1))
#define SEQ_MAX      (NRF_FICR->CODEPAGESIZE * 8 * SEQ_PER_BIT)

static struct device *gpio;
static struct device *nvm;
static struct device *pwm;

static struct k_work button_work;

static void button_send_pressed(struct k_work *work)
{
	printk("button_send_pressed()\n");
	board_button_1_pressed();
}

static void button_pressed(struct device *dev, struct gpio_callback *cb,
			   u32_t pins)
{
	if (pins & BIT(DT_ALIAS_SW0_GPIOS_PIN)) {
		k_work_submit(&button_work);
	} else {
		u16_t target = board_set_target();

		if (target > 0x0009) {
			printk("A");
		} else {
			printk("%x", (target & 0xf));
		}
	}
}

void board_play_tune(const char *str)
{
	printk("play tune: %s\n", str);
}

void board_heartbeat(u8_t hops, u16_t feat)
{
	printk("%u hops\n", hops);
}

void board_other_dev_pressed(u16_t addr)
{
	printk("board_other_dev_pressed(0x%04x)\n", addr);
}

void board_attention(bool attention)
{
	if (attention) {
		printk("Attention!\n");
	}
}

static void configure_button(void)
{
	static struct gpio_callback button_cb;

	k_work_init(&button_work, button_send_pressed);

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

void board_init(u16_t *addr)
{
	nvm = device_get_binding(DT_FLASH_DEV_NAME);
	pwm = device_get_binding(DT_INST_0_NORDIC_NRF_PWM_LABEL);

	*addr = NRF_UICR->CUSTOMER[0];
	if (!*addr || *addr == 0xffff) {
#if defined(NODE_ADDR)
		*addr = NODE_ADDR;
#else
		*addr = 0x0b0c;
#endif
	}

	configure_button();
}
