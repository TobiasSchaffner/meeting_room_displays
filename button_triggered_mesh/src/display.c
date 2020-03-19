/*
 * Copyright (c) 2018 Jan Van Winkel <jan.van_winkel@dxplore.eu>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>
#include <drivers/display.h>
#include <sys/printk.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <zephyr.h>

#include "display.h"

struct device *display_dev;

lv_obj_t *top_label;
lv_obj_t *center_label;
lv_obj_t *bottom_label;

static void configure_center_label(void){
    center_label = lv_label_create(lv_scr_act(), NULL);
	lv_obj_align(center_label, NULL, LV_ALIGN_CENTER, 0, 0);
}

static void configure_top_label(void) {
	top_label = lv_label_create(lv_scr_act(), NULL);
	lv_obj_align(top_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
}

static void configure_bottom_label(void) {
	bottom_label = lv_label_create(lv_scr_act(), NULL);
	lv_obj_align(bottom_label, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
}

static void configure_labels(void) {
    configure_top_label();
    configure_center_label();
    configure_bottom_label();
}

void display_init(void)
{
	display_dev = device_get_binding(CONFIG_LVGL_DISPLAY_DEV_NAME);

	if (display_dev == NULL) {
		printk("Device not found.");
		return;
	}

    configure_labels();

	lv_task_handler();
	display_blanking_off(display_dev);
}

void display_set_top_label(char* message) {
    lv_label_set_text(top_label, message);
    lv_task_handler();
}

void display_set_center_label(char* message) {
    lv_label_set_text(center_label, message);
    lv_task_handler();
}

void display_set_bottom_label(char* message) {
    lv_label_set_text(bottom_label, message);
    lv_task_handler();
}
