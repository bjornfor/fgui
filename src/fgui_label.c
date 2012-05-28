#include <stdint.h>
#include <string.h>

#include "fgui_label.h"
#include "fgui_font.h"

#define LABEL_TEXT_COLOR 0


void fgui_label_init(struct fgui_label *label, uint16_t x, uint16_t y, const
		const char *text)
{
	label->x = x;
	label->y = y;
	fgui_label_set_text(label, text);
}

void fgui_label_set_text(struct fgui_label *label, const char *text)
{
	strncpy(label->text, text, sizeof label->text);
	label->text[sizeof label->text - 1] = '\0';
}

void fgui_label_draw(struct fgui_label *label)
{
	fgui_draw_string(label->text, label->x, label->y, LABEL_TEXT_COLOR);
}
