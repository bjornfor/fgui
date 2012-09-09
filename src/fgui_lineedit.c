/*
 * Copyright (c) 2012 Bjørn Forsman
 *
 * This file is part of fgui which is released under the MIT license. See
 * LICENSE.txt or go to http://www.opensource.org/licenses/MIT for full
 * license text.
 */

#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "fgui_event.h"
#include "fgui_widget.h"
#include "fgui_lineedit.h"
#include "fgui_primitives.h"
#include "fgui_font.h"
#include "fgui_font_data.h"

#define FGUI_LINEEDIT_FOCUS_COLOR (0xff << 16)
#define FGUI_LINEEDIT_BORDER_COLOR 0xaaaaaaaa
#define FGUI_LINEEDIT_BG_COLOR 0x00f0f0f0
#define FGUI_LINEEDIT_TEXT_COLOR 0

int fgui_lineedit_init(struct fgui_lineedit *lineedit,
		uint16_t x, uint16_t y, uint16_t w, uint16_t h,
		struct fgui_widget *parent)
{
	int ret;

	ret = fgui_widget_init(&lineedit->base, x, y, w, h, parent);
	if (ret != 0) {
		return -1;
	}

	fgui_widget_set_draw((struct fgui_widget *)lineedit, fgui_lineedit_draw);
	lineedit->base.event_handler = fgui_lineedit_event_handler;
	lineedit->cursor = 0;
	lineedit->text[0] = '\0';
	return 0;
}

void fgui_lineedit_draw(struct fgui_widget *widget)
{
	struct fgui_lineedit *lineedit = (struct fgui_lineedit *)widget;
	size_t char_width;
	size_t char_height;

	/* draw light background */
	fgui_fill_rectangle(lineedit->base.area.x, lineedit->base.area.y, lineedit->base.area.w,
			lineedit->base.area.h, FGUI_LINEEDIT_BG_COLOR);

	/* draw widget border (color depends on focus) */
	if (lineedit->base.has_focus) {
		fgui_draw_rectangle(lineedit->base.area.x, lineedit->base.area.y, lineedit->base.area.w,
				lineedit->base.area.h, FGUI_LINEEDIT_FOCUS_COLOR);
	} else {
		fgui_draw_rectangle(lineedit->base.area.x, lineedit->base.area.y, lineedit->base.area.w,
				lineedit->base.area.h, FGUI_LINEEDIT_BORDER_COLOR);
	}

	/* draw text */
	fgui_draw_string(lineedit->text, lineedit->base.area.x + 4, lineedit->base.area.y + 4,
			FGUI_LINEEDIT_TEXT_COLOR, &lineedit->base.area);

	/* draw cursor (if we have focus) */
	if (lineedit->base.has_focus) {
		/* assume monospaced font */
		char_width = cWidth[0];
		char_height = cHeight[0];
		fgui_draw_line(lineedit->base.area.x + 4 + lineedit->cursor * char_width,
				lineedit->base.area.y + 2,
				lineedit->base.area.x + 4 + lineedit->cursor * char_width,
				lineedit->base.area.y + 2 + char_height,
				FGUI_LINEEDIT_TEXT_COLOR);
	}
}

static void delete_char(char *str, size_t index)
{
	size_t i;
	size_t len;

	len = strlen(str);
	if (len == 0) {
		return;
	}

	for (i = index; i < len; i++) {
		str[i] = str[i + 1];
	}
}

static void insert_char(char *str, size_t index, char ch)
{
	size_t i;
	size_t len;

	len = strlen(str);
	if (len == 0) {
		str[0] = ch;
		str[1] = '\0';
		return;
	}

	for (i = len; /* handled in body */; i--) {
		str[i + 1] = str[i];
		if (i == 0 || i < index) {
			break;
		}
	}
	str[index] = ch;
}

int fgui_lineedit_event_handler(struct fgui_widget *widget, struct fgui_event *event)
{
	struct fgui_lineedit *lineedit = (struct fgui_lineedit *)widget;
	int ret = 1;

	if (event->type == FGUI_EVENT_KEYDOWN && isprint(event->key.keycode)) {
		if (strlen(lineedit->text) < FGUI_LINEEDIT_MAX_TEXTLEN - 1) {
			insert_char(lineedit->text, lineedit->cursor, event->key.keycode);
			lineedit->cursor++;
		}
		ret = 0;
	} else if (event->type == FGUI_EVENT_KEYDOWN &&
			(event->key.keycode == FGUI_KEY_BACKSPACE)) {
		/* backspace moves the cursor back one step and then deletes
		 * like 'delete' */
		if (lineedit->cursor > 0) {
			lineedit->cursor--;
			delete_char(lineedit->text, lineedit->cursor);
		}
		ret = 0;
	} else if (event->type == FGUI_EVENT_KEYDOWN &&
			(event->key.keycode == FGUI_KEY_DELETE)) {
		/* delete removes the char after the cursor and leaves the
		 * cursor alone */
		delete_char(lineedit->text, lineedit->cursor);
		ret = 0;
	} else if (event->type == FGUI_EVENT_KEYDOWN && (event->key.keycode == FGUI_KEY_ARROW_LEFT)) {
		if (lineedit->cursor > 0) {
			lineedit->cursor--;
		}
		ret = 0;
	} else if (event->type == FGUI_EVENT_KEYDOWN && (event->key.keycode == FGUI_KEY_ARROW_RIGHT)) {
		if (lineedit->text[lineedit->cursor] != '\0') {
			lineedit->cursor++;
		}
		ret = 0;
	}

	return ret;
}

void fgui_lineedit_get_text(struct fgui_lineedit *lineedit, char *text)
{
	strncpy(text, lineedit->text, FGUI_LINEEDIT_MAX_TEXTLEN - 1);
	text[FGUI_LINEEDIT_MAX_TEXTLEN - 1] = '\0';
}
