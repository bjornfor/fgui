/*
 * Copyright (c) 2012 Bjørn Forsman
 *
 * This file is part of fgui which is released under the MIT license. See
 * LICENSE.txt or go to http://www.opensource.org/licenses/MIT for full
 * license text.
 */

/**
 * @file
 *
 * fgui push button
 */

#ifndef FGUI_BUTTON_H
#define FGUI_BUTTON_H

#include <stdint.h>
#include <stdbool.h>

#include "fgui_widget.h"

#define MAX_TEXT_LEN 50

struct fgui_button {
	struct fgui_widget base;
	uint16_t width;
	uint16_t height;
	char text[MAX_TEXT_LEN];
	void (*on_click)(void *userdata);
	void *on_click_userdata;
	bool is_depressed;
};

void fgui_button_init(struct fgui_button *button, uint16_t x, uint16_t y,
		uint16_t w, uint16_t h, const char *text, struct fgui_widget *parent);
void fgui_button_set_text(struct fgui_button *button, const char *text);
void fgui_button_draw(struct fgui_widget *widget);
void fgui_button_set_on_click_handler(struct fgui_button *button,
		void (*callback)(void *userdata), void *userdata);
int fgui_button_event_handler(struct fgui_widget *widget, struct fgui_event *event);

#endif /* FGUI_BUTTON_H */
