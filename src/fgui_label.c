#include <stdint.h>
#include <string.h>

#include "fgui_label.h"
#include "fgui_font.h"

#define LABEL_TEXT_COLOR 0


void fgui_label_init(struct fgui_label *label, uint16_t x, uint16_t y, const
		const char *text, struct fgui_widget *parent)
{
	fgui_widget_init((struct fgui_widget *)label, parent);
	label->base.focus_policy = FGUI_NO_FOCUS;
	label->base.x = x;
	label->base.y = y;
	fgui_label_set_text(label, text);
}

void fgui_label_set_text(struct fgui_label *label, const char *text)
{
	strncpy(label->text, text, sizeof label->text);
	label->text[sizeof label->text - 1] = '\0';
}

void fgui_label_draw(struct fgui_widget *widget)
{
	struct fgui_label *label = (struct fgui_label *)widget;
	fgui_draw_string(label->text, label->base.x, label->base.y, LABEL_TEXT_COLOR);
}
