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
 * fgui lineedit - single line text input widget
 */

#ifndef FGUI_LINEEDIT_H
#define FGUI_LINEEDIT_H

#include "fgui_event.h"
#include "fgui_widget.h"

#define FGUI_LINEEDIT_MAX_TEXTLEN 50

struct fgui_lineedit {
	struct fgui_widget base;
	uint16_t width;
	uint16_t height;
	char text[FGUI_LINEEDIT_MAX_TEXTLEN];
	size_t cursor;
};


int fgui_lineedit_init(struct fgui_lineedit *lineedit, uint16_t x, uint16_t y, uint16_t w, uint16_t h, struct fgui_widget *parent);
void fgui_lineedit_draw(struct fgui_widget *widget);
int fgui_lineedit_event_handler(struct fgui_widget *widget, struct fgui_event *event);
void fgui_lineedit_get_text(struct fgui_lineedit *lineedit, char *text);

#endif /* FGUI_LINEEDIT_H */
