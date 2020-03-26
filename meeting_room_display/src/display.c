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
#include "images.h"

#define SCREEN_WIDTH    800
#define SCREEN_HEIGTH   480
#define PADDING         20
#define WINDOW_WIDTH    ((SCREEN_WIDTH - (3 * PADDING)) / 2)
#define WINDOW_HEIGHT   (SCREEN_HEIGTH - (2 * PADDING))

static struct device *display_dev;

// Wallpaper
static lv_obj_t* wallpaper;

// Windows common
static lv_style_t style_window;

// Main Window
static lv_obj_t* main_window;

static lv_obj_t* main_logo;

// Status Window
static lv_obj_t* status_window;

static lv_obj_t* status_address_label;
static lv_obj_t* status_message_label;
static lv_obj_t* status_hearthbeat_label;

// Calendar Windows
static lv_obj_t* calendar_window;

static lv_style_t style_header;
static lv_obj_t* calendar_header;
static lv_obj_t* calendar_header_label;

// Time Slots
static lv_style_t style_time_slot;
static lv_obj_t* calendar_time_slots[10];
static lv_obj_t* calendar_time_slot_labels[10];

static lv_style_t style_line;
static lv_point_t line_points[] = {{0, 0}, {305, 0}};

static void create_styles(void) {
    lv_style_copy(&style_window, &lv_style_plain);
    style_window.body.border.width = 3;

    lv_style_copy(&style_header, &lv_style_plain);
    style_header.body.border.width = 1;
    style_header.text.font = &lv_font_roboto_22;

    lv_style_copy(&style_time_slot, &lv_style_plain);
    style_time_slot.body.border.width = 1;

    lv_style_copy(&style_line, &lv_style_plain);
    style_line.line.width = 1;
}

static void set_styles(void) {
    lv_obj_set_style(calendar_window, &style_window);
    lv_obj_set_style(main_window, &style_window);
    lv_obj_set_style(status_window, &style_window);

    lv_task_handler();
}

static void create_wallpaper(void) {
    LV_IMG_DECLARE(chess);
    wallpaper = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(wallpaper, &chess);
    lv_obj_set_size(wallpaper, 800, 480);
    lv_task_handler();
}

static void create_main_window() {
    main_window = lv_obj_create(wallpaper, NULL);
    lv_obj_set_size(main_window, WINDOW_WIDTH, 360);
    lv_obj_align(main_window, NULL, LV_ALIGN_IN_TOP_RIGHT, -20, 20);

    LV_IMG_DECLARE(mixedmode);
    main_logo = lv_img_create(main_window, NULL);
    lv_img_set_src(main_logo, &mixedmode);
    lv_obj_align(main_logo, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -7, -3);
}

static void create_status_window(void) {
    status_window = lv_obj_create(wallpaper, NULL);
    lv_obj_set_size(status_window, WINDOW_WIDTH, 60);
    lv_obj_align(status_window, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -20, -20);

    status_message_label = lv_label_create(status_window, NULL);
    lv_label_set_text(status_message_label, "Status:");
	lv_obj_align(status_message_label, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);

	status_address_label = lv_label_create(status_window, NULL);
    lv_label_set_text(status_address_label, "Address:");
	lv_obj_align(status_address_label, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0);

    status_hearthbeat_label = lv_label_create(status_window, NULL);
    lv_label_set_text(status_hearthbeat_label, "Hops:");
	lv_obj_align(status_hearthbeat_label, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10);
}

static void create_calendar(void) {
    char buffer[20];

    calendar_window = lv_obj_create(wallpaper, NULL);
    lv_obj_set_size(calendar_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	lv_obj_align(calendar_window, NULL, LV_ALIGN_IN_TOP_LEFT, PADDING, PADDING);

    calendar_header = lv_obj_create(calendar_window, NULL);
    lv_obj_set_style(calendar_header, &style_header);
    lv_obj_set_size(calendar_header, WINDOW_WIDTH - 4, 34);
    lv_obj_align(calendar_header, NULL, LV_ALIGN_IN_TOP_LEFT, 2, 2);

	calendar_header_label = lv_label_create(calendar_header, NULL);
    lv_label_set_text(calendar_header_label, "Montag, 19.02.20");
    lv_obj_align(calendar_header_label, NULL, LV_ALIGN_CENTER, 0, 0);


    for (int time_slot = 0; time_slot < 10; time_slot++) {
        calendar_time_slots[time_slot] = lv_obj_create(wallpaper, NULL);
        lv_obj_set_size(calendar_time_slots[time_slot], WINDOW_WIDTH - 4, 41);
        lv_obj_set_style(calendar_time_slots[time_slot], &style_time_slot);
        lv_obj_align(calendar_time_slots[time_slot], NULL, LV_ALIGN_IN_TOP_LEFT, PADDING + 2, PADDING + 37 + (40 * time_slot));

        calendar_time_slot_labels[time_slot] = lv_label_create(calendar_time_slots[time_slot], NULL);
	    lv_obj_align(calendar_time_slot_labels[time_slot], NULL, LV_ALIGN_IN_TOP_LEFT, 5, 7);

        lv_obj_t* line = lv_line_create(calendar_time_slots[time_slot], NULL);
        lv_line_set_points(line, line_points, 2);
        lv_line_set_style(line, LV_LINE_STYLE_MAIN, &style_line);
        lv_obj_align(line, NULL, LV_ALIGN_IN_TOP_LEFT, 60, 20);
        lv_line_set_auto_size(line, false);

		sprintf(buffer, "%02d:00", 8 + time_slot);
        lv_label_set_text(calendar_time_slot_labels[time_slot], buffer);
    }
}

static void create_windows(void) {
    create_wallpaper();
    create_calendar();
    create_main_window();
    create_status_window();
    lv_task_handler();
}

void display_init(void)
{
	display_dev = device_get_binding(CONFIG_LVGL_DISPLAY_DEV_NAME);

	if (display_dev == NULL) {
		printk("Device not found.");
		return;
	}

    create_styles();
    create_windows();

    display_blanking_off(display_dev);

    set_styles();
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
