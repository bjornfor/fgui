/**
 * @file
 *
 * fgui lineedit - single line text input widget
 */

#ifndef FGUI_LINEEDIT_H
#define FGUI_LINEEDIT_H

#include "fgui_widget.h"

#define FGUI_LINEEDIT_MAX_TEXTLEN 50

struct fgui_lineedit {
	struct fgui_widget base;
	uint16_t width;
	uint16_t height;
	char text[FGUI_LINEEDIT_MAX_TEXTLEN];
	size_t cursor;
};


void fgui_lineedit_init(struct fgui_lineedit *lineedit, uint16_t x, uint16_t y, uint16_t w, uint16_t h, struct fgui_widget *parent);
void fgui_lineedit_draw(struct fgui_widget *widget);
int fgui_lineedit_event_handler(struct fgui_widget *widget, struct fgui_event *event);

#endif /* FGUI_LINEEDIT_H */
