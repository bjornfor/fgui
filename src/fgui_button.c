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
 * fgui button module
 */

#include <stdint.h>
#include <string.h>

#include "fgui_event.h"
#include "fgui_widget.h"
#include "fgui_button.h"
#include "fgui_primitives.h"
#include "fgui_font.h"

#define FGUI_BUTTON_BG_COLOR 0xcccccccc
#define FGUI_BUTTON_BORDER_COLOR 0xaaaaaaaa
#define FGUI_BUTTON_TEXT_COLOR 0
#define FGUI_BUTTON_FOCUS_COLOR (0xff << 16)

void fgui_button_init(struct fgui_button *button, uint16_t x, uint16_t y,
		uint16_t w, uint16_t h, const char *text, struct fgui_widget *parent)
{
	// cast or use button.base as argument
	fgui_widget_init(&button->base, parent);
	fgui_widget_set_draw(&button->base, fgui_button_draw);

	button->base.x = x;
	button->base.y = y;
	button->base.event_handler = fgui_button_event_handler;
	button->width = w;
	button->height = h;
	fgui_button_set_text(button, text);
	button->is_depressed = false;
}

void fgui_button_set_text(struct fgui_button *button, const char *text)
{
	strncpy(button->text, text, sizeof button->text);
	button->text[sizeof button->text - 1] = '\0';
}

void fgui_button_draw(struct fgui_widget *widget)
{
	struct fgui_button *button = (struct fgui_button *)widget;

	/* button background */
	fgui_fill_rectangle(button->base.x, button->base.y, button->width,
			button->height, FGUI_BUTTON_BG_COLOR);

	/* draw border, change color depending on focus */
	if (button->base.has_focus) {
		fgui_draw_rectangle(button->base.x, button->base.y, button->width,
				button->height, FGUI_BUTTON_FOCUS_COLOR);
	} else {
		fgui_draw_rectangle(button->base.x, button->base.y, button->width,
				button->height, FGUI_BUTTON_BORDER_COLOR);
	}

	/* button text */
	fgui_draw_string(button->text, button->base.x+2, button->base.y+2,
			FGUI_BUTTON_TEXT_COLOR);

	/* draw extra thick border if button is depressed */
	if (button->is_depressed) {
		fgui_draw_rectangle(button->base.x-1, button->base.y-1,
				button->width+2, button->height+2,
				FGUI_BUTTON_FOCUS_COLOR);
	}

}

void fgui_button_set_on_click_handler(struct fgui_button *button,
		void (*callback)(void *userdata), void *userdata)
{
	button->on_click = callback;
	button->on_click_userdata = userdata;
}

int fgui_button_event_handler(struct fgui_widget *widget, struct fgui_event *event)
{
	struct fgui_button *button = (struct fgui_button *)widget;

	// keycode 0x09 == TAB, 0x20 == SPACE
	if (event->type == FGUI_EVENT_KEYUP && event->key.keycode == 0x20) {
		if (button->on_click) {
			button->on_click(button->on_click_userdata);
		}
		button->is_depressed = false;
		return 0;
	}

	if (event->type == FGUI_EVENT_KEYDOWN && event->key.keycode == 0x20) {
		button->is_depressed = true;
		return 0;
	}

	return 1;
}
