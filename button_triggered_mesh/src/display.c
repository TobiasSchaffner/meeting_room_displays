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

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_CHESS
#define LV_ATTRIBUTE_IMG_CHESS
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_IMG_CHESS uint8_t chess_map[] = {
  /*Pixel format: Red: 3 bit, Green: 3 bit, Blue: 2 bit*/
  0x00, 0xff,
  0xff, 0x00,
};

const lv_img_dsc_t chess = {
  .header.always_zero = 0,
  .header.w = 2,
  .header.h = 2,
  .data_size = 4 * LV_COLOR_SIZE / 8,
  .header.cf = LV_IMG_CF_TRUE_COLOR,
  .data = chess_map,
};

static struct device *display_dev;

// Wallpaper
static lv_obj_t* wallpaper;

// Windows common
static lv_style_t style_window;

// Status Window
static lv_obj_t* status_window;

static lv_obj_t* status_address_label;
static lv_obj_t* status_message_label;
static lv_obj_t* status_hearthbeat_label;

static void create_styles(void) {
    lv_style_copy(&style_window, &lv_style_plain);
    style_window.body.border.width = 3;
    style_window.body.main_color = LV_COLOR_WHITE;
    //style_window.text.font = &lv_font_roboto_16;
}

static void create_wallpaper() {
    LV_IMG_DECLARE(chess);
    wallpaper = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(wallpaper, &chess);
    lv_obj_set_size(wallpaper, 800, 480);
    lv_task_handler();
}

static void create_status_window(void) {
    status_window = lv_obj_create(wallpaper, NULL);
    lv_obj_set_size(status_window, 200, 100);
    lv_obj_set_style(status_window, &style_window);
    lv_obj_align(status_window, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 10);

	status_address_label = lv_label_create(status_window, NULL);
	lv_obj_align(status_address_label, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);

    status_message_label = lv_label_create(status_window, NULL);
	lv_obj_align(status_message_label, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0);

    status_hearthbeat_label = lv_label_create(status_window, NULL);
	lv_obj_align(status_hearthbeat_label, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10);
}

static void create_gui(void) {
    create_styles();
    create_wallpaper();
    create_status_window();
}

void display_init(void)
{
	display_dev = device_get_binding(CONFIG_LVGL_DISPLAY_DEV_NAME);

	if (display_dev == NULL) {
		printk("Device not found.");
		return;
	}

    create_gui();

	lv_task_handler();
    display_blanking_off(display_dev);
}

void display_set_top_label(char* message) {
    lv_label_set_text(status_message_label, message);
    lv_task_handler();
}

void display_set_center_label(char* message) {
    lv_label_set_text(status_address_label, message);
    lv_task_handler();
}

void display_set_bottom_label(char* message) {
    lv_label_set_text(status_hearthbeat_label, message);
    lv_task_handler();
}
