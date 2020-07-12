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

#define APPOINTMENT_SLOTS  10

static struct device *display_dev;

// Wallpaper
static lv_obj_t* wallpaper;

// Windows common
static lv_style_t style_window;

// Main Window
static lv_obj_t* main_window;

static lv_obj_t* main_room_label;
static lv_obj_t* main_logo;

static char main_room_message[24] = {0};

// Status Window
static lv_obj_t* status_window;
static lv_style_t style_status_window;

static lv_obj_t* status_address_label;
static lv_obj_t* status_message_label;
static lv_obj_t* status_heartbeat_label;

static char status_address_message[24] = {0};
static char status_message_message[24] = {0};
static char status_heartbeat_message[24] = {0};

// Calendar Windows
static lv_obj_t* calendar_window;

static lv_style_t style_header;
static lv_obj_t* calendar_header;
static lv_obj_t* calendar_header_label;

static char calender_header_message[24] = {0};

// Time Slots
static lv_style_t style_time_slot;
static lv_obj_t* calendar_time_slots[10];
static lv_obj_t* calendar_time_slot_labels[10];

static lv_style_t style_line;
static lv_point_t line_points[] = {{0, 0}, {305, 0}};

// Appointments
static lv_style_t style_appointment;
static int appointments;
static lv_obj_t* calendar_appointment_slots[APPOINTMENT_SLOTS];
static lv_obj_t* calendar_appointment_labels[APPOINTMENT_SLOTS];

static void create_styles(void) {
    lv_style_copy(&style_window, &lv_style_plain);
    style_window.body.border.width = 3;
    style_window.text.font = &lv_font_roboto_28;

    lv_style_copy(&style_status_window, &lv_style_plain);
    style_status_window.body.border.width = 3;

    lv_style_copy(&style_header, &lv_style_plain);
    style_header.body.border.width = 1;
    style_header.text.font = &lv_font_roboto_22;

    lv_style_copy(&style_time_slot, &lv_style_plain);
    style_time_slot.body.border.width = 1;

    lv_style_copy(&style_line, &lv_style_plain);
    style_line.line.width = 1;

    lv_style_copy(&style_appointment, &lv_style_plain);
    style_appointment.body.main_color = LV_COLOR_BLACK;
    style_appointment.body.grad_color = LV_COLOR_BLACK;
    style_appointment.body.radius = 10;
    style_appointment.text.color = LV_COLOR_WHITE;
}

static void set_styles(void) {
    lv_obj_set_style(calendar_window, &style_window);
    lv_obj_set_style(main_window, &style_window);
    lv_obj_set_style(status_window, &style_status_window);

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

    main_room_label = lv_label_create(main_window, NULL);
    lv_label_set_text(main_room_label, "Room: 0.000");
    lv_obj_align(main_room_label, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);

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

    status_heartbeat_label = lv_label_create(status_window, NULL);
    lv_label_set_text(status_heartbeat_label, "Hops:");
	lv_obj_align(status_heartbeat_label, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10);
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
    lv_label_set_text(calendar_header_label, "Mo, 01.01.01");
    lv_obj_align(calendar_header_label, NULL, LV_ALIGN_CENTER, 0, 0);


    for (int time_slot = 0; time_slot < 10; time_slot++) {
        calendar_time_slots[time_slot] = lv_obj_create(calendar_window, NULL);
        lv_obj_set_size(calendar_time_slots[time_slot], WINDOW_WIDTH - 4, 41);
        lv_obj_set_style(calendar_time_slots[time_slot], &style_time_slot);
        lv_obj_align(calendar_time_slots[time_slot], NULL, LV_ALIGN_IN_TOP_LEFT, 2, 2 + 35 + (40 * time_slot));

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

int display_init(void)
{
	display_dev = device_get_binding(CONFIG_LVGL_DISPLAY_DEV_NAME);

	if (display_dev == NULL) {
		printk("Device not found.");
		return 1;
	}

    create_styles();
    create_windows();

    display_blanking_off(display_dev);

    set_styles();

    return 0;
}

void display_create_appointment(message_appointment* appointment) {
    if (appointment >= APPOINTMENT_SLOTS ||
        appointment->start >= appointment->end ||
        appointment->start < 8 ||
        appointment->end > 18)
            return;

    printk("Adding Apointment %s\n", appointment->description);

    int slot = appointments;

    int halign = 60;
    int valign = 4 + 35 + (40 * (appointment->start - 8));
    int width = WINDOW_WIDTH - halign - 4;
    int height = 40 * (appointment->end - appointment->start) - 3;

    calendar_appointment_slots[slot] = lv_obj_create(calendar_window, NULL);
    lv_obj_set_style(calendar_appointment_slots[slot], &style_appointment);
    lv_obj_set_size(calendar_appointment_slots[slot], width, height);
    lv_obj_align(calendar_appointment_slots[slot], NULL, LV_ALIGN_IN_TOP_LEFT, halign, valign);

	calendar_appointment_labels[slot] = lv_label_create(calendar_appointment_slots[slot], NULL);
    lv_label_set_text(calendar_appointment_labels[slot], appointment->description);
    lv_obj_align(calendar_appointment_labels[slot], NULL, LV_ALIGN_CENTER, 0, 0);

    appointments++;
    lv_task_handler();
}

void display_clear_appointments(void) {
    for (int slot = 0; slot < appointments; slot++) {
        lv_obj_del(calendar_appointment_labels[slot]);
        lv_obj_del(calendar_appointment_slots[slot]);
    }
    appointments = 0;
    lv_task_handler();
}

void display_set_status_message(const char* message) {
    snprintf(status_message_message, 24, "Status: %s", message);
    lv_label_set_text(status_message_label, status_message_message);
    lv_task_handler();
}

void display_set_status_address(uint16_t address) {
    snprintf(status_address_message, 24, "Address: 0x%04x", address);
    lv_label_set_text(status_address_label, status_address_message);
    lv_task_handler();
}

void display_set_status_heartbeat(uint16_t hops) {
    snprintf(status_heartbeat_message, 24, "Hops: %d", hops);
    lv_label_set_text(status_heartbeat_label, status_heartbeat_message);
    lv_task_handler();
}

void display_set_date(const char* message) {
    snprintf(calender_header_message, 24, "%s", message);
    lv_label_set_text(calendar_header_label, message);
    lv_task_handler();
}

void display_set_room(const char* message) {
    snprintf(main_room_message, 24, "Room: %s", message);
    lv_label_set_text(main_room_label, main_room_message);
    lv_task_handler();
}