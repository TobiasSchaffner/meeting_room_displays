/* board.h - Board-specific hooks */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if !defined(NODE_ADDR)
#define NODE_ADDR 0x0b0c
#endif

void on_button_1_press(void);
void on_button_2_press(void);

void board_init(u16_t *addr);