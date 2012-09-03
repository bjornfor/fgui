/*
 * Copyright (c) 2012 Bjørn Forsman
 *
 * This file is part of fgui which is released under the MIT license. See
 * LICENSE.txt or go to http://www.opensource.org/licenses/MIT for full
 * license text.
 */

#include <stdint.h>
#include <stddef.h>    // size_t
#include <string.h>

#include "fgui_event.h"
#include "fgui_widget.h"
#include "fgui_combobox.h"
#include "fgui_primitives.h"
#include "fgui_font.h"

#define FGUI_COMBOBOX_BG_COLOR 0xcccccccc
#define FGUI_COMBOBOX_BORDER_COLOR 0xaaaaaaaa
#define FGUI_COMBOBOX_HIGHLIGHT_COLOR 0x00909090
#define FGUI_COMBOBOX_TEXT_COLOR 0
#define FGUI_COMBOBOX_FOCUS_COLOR (0xff << 16)


int fgui_combobox_init(struct fgui_combobox *combobox, uint16_t x, uint16_t y,
		uint16_t w, uint16_t h, struct fgui_widget *parent)
{
	int ret;

	ret = fgui_widget_init((struct fgui_widget *)combobox, parent);
	if (ret != 0) {
		return -1;
	}

	fgui_widget_set_draw((struct fgui_widget *)combobox, fgui_combobox_draw);
	combobox->base.x = x;
	combobox->base.y = y;
	combobox->num_items = 0;
	combobox->current_item = -1;
	combobox->width = w;
	combobox->height = h;
	combobox->base.event_handler = fgui_combobox_event_handler;
	return 0;
}

int fgui_combobox_add_item(struct fgui_combobox *combobox, const char *text)
{
	size_t i;

	if (combobox->num_items >= MAX_COMBOBOX_ITEMS) {
		return -1;
	}

	i = combobox->num_items;
	strncpy(combobox->items[i].text, text, MAX_COMBOBOX_TEXT_LEN - 1);
	combobox->items[i].text[MAX_COMBOBOX_TEXT_LEN - 1] = '\0';
	combobox->num_items++;
	return 0;
}

int fgui_combobox_remove_item(struct fgui_combobox *combobox, size_t index)
{
	size_t i;

	if (index >= combobox->num_items) {
		return -1;
	}

	/* remove item by shifting the other items down one step */
	for (i = index; i < combobox->num_items - 1; i++) {
		combobox->items[i] = combobox->items[i + 1];
	}
	combobox->num_items--;
	return 0;
}

int fgui_combobox_current_index(struct fgui_combobox *combobox)
{
	return combobox->current_item;
}

void fgui_combobox_current_text(struct fgui_combobox *combobox, char *text)
{
	if (combobox->current_item < 0) {
		strcpy(text, "");
	} else {
		strncpy(text, combobox->items[combobox->current_item].text, MAX_COMBOBOX_TEXT_LEN - 1);
		text[MAX_COMBOBOX_TEXT_LEN - 1] = '\0';
	}
}

void fgui_combobox_set_index(struct fgui_combobox *combobox, size_t index)
{
	/* bound index to our available items */
	if (index < combobox->num_items) {
		combobox->current_item = index;
	} else {
		combobox->current_item = combobox->num_items - 1;
	}
}

void fgui_combobox_draw(struct fgui_widget *widget)
{
	size_t i;
	struct fgui_combobox *combobox = (struct fgui_combobox *)widget;

	if (!combobox->base.has_focus) {
		combobox->is_expanded = false;
	}

	if (combobox->is_expanded) {
		/* combobox background */
		fgui_fill_rectangle(combobox->base.x, combobox->base.y,
				combobox->width, combobox->height * combobox->num_items,
				FGUI_COMBOBOX_BG_COLOR);

		/* highlight current item */
		fgui_fill_rectangle(combobox->base.x,
				combobox->base.y + combobox->height * combobox->current_item,
				combobox->width,
				combobox->height,
				FGUI_COMBOBOX_HIGHLIGHT_COLOR);

		/* border */
		fgui_draw_rectangle(combobox->base.x, combobox->base.y,
				combobox->width, combobox->height * combobox->num_items,
				FGUI_COMBOBOX_FOCUS_COLOR);

		for (i = 0; i < combobox->num_items; i++) {
			/* current item text */
			fgui_draw_string(combobox->items[i].text,
					combobox->base.x+2,
					combobox->base.y+2 + combobox->height * i,
					FGUI_COMBOBOX_TEXT_COLOR);
		}
	} else {
		/* combobox background */
		fgui_fill_rectangle(combobox->base.x, combobox->base.y,
				combobox->width, combobox->height,
				FGUI_COMBOBOX_BG_COLOR);

		/* border */
		fgui_draw_rectangle(combobox->base.x, combobox->base.y,
				combobox->width, combobox->height,
				FGUI_COMBOBOX_BORDER_COLOR);


		/* if focus, draw red border */
		if (combobox->base.has_focus) {
			fgui_draw_rectangle(combobox->base.x, combobox->base.y,
					combobox->width, combobox->height,
					FGUI_COMBOBOX_FOCUS_COLOR);
		}

		/* current item text */
		fgui_draw_string(combobox->items[combobox->current_item].text,
				combobox->base.x+2, combobox->base.y+2,
				FGUI_COMBOBOX_TEXT_COLOR);

		/* draw arrow to indicate that this is a combobox */
		fgui_draw_triangle(combobox->base.x + combobox->width - 15,
				combobox->base.y + combobox->height / 2 - 2,
				combobox->base.x + combobox->width - 5,
				combobox->base.y + combobox->height / 2 - 2,
				combobox->base.x + combobox->width - 10,
				combobox->base.y + combobox->height / 2 + 3,
				FGUI_COMBOBOX_BORDER_COLOR);
	}
}

int fgui_combobox_event_handler(struct fgui_widget *widget, struct fgui_event *event)
{
	struct fgui_combobox *combobox = (struct fgui_combobox *)widget;

	if (event->type == FGUI_EVENT_KEYDOWN && event->key.keycode == FGUI_KEY_SPACE) {
		combobox->is_expanded = !combobox->is_expanded;
		return 0;
	}

	if (event->type == FGUI_EVENT_KEYDOWN && event->key.keycode == FGUI_KEY_ARROW_DOWN) {
		if (combobox->current_item < combobox->num_items - 1) {
			combobox->current_item++;
			if (combobox->on_change) {
				combobox->on_change(combobox->on_change_userdata);
			}
		}
		return 0;
	}

	if (event->type == FGUI_EVENT_KEYDOWN && event->key.keycode == FGUI_KEY_ARROW_UP) {
		if (combobox->current_item > 0) {
			combobox->current_item--;
			if (combobox->on_change) {
				combobox->on_change(combobox->on_change_userdata);
			}
		}
		return 0;
	}

	if (event->type == FGUI_EVENT_KEYDOWN && event->key.keycode == FGUI_KEY_RETURN) {
		combobox->is_expanded = false;
		return 0;
	}

	/* we didn't handle the event */
	return 1;
}

void fgui_combobox_set_on_change_handler(struct fgui_combobox *combobox,
		void (*callback)(void *userdata), void *userdata)
{
	combobox->on_change = callback;
	combobox->on_change_userdata = userdata;
}
