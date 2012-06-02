/**
 * @file
 *
 * fgui text label, supports '\n' to create multiline labels
 */

#ifndef FGUI_LABEL_H
#define FGUI_LABEL_H

#include <stdint.h>

#include "fgui_widget.h"

#define MAX_TEXT_LEN 50

struct fgui_label {
	struct fgui_widget base;
	char text[MAX_TEXT_LEN];
};

void fgui_label_init(struct fgui_label *label, uint16_t x, uint16_t y, const
		const char *text, struct fgui_widget *parent);
void fgui_label_set_text(struct fgui_label *label, const char *text);
void fgui_label_draw(struct fgui_widget *widget);

#endif /* FGUI_LABEL_H */
