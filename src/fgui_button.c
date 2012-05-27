
/**
 * @file
 *
 * fgui button module
 */

#include <stdint.h>
#include <string.h>

#include "fgui_button.h"
#include "fgui_primitives.h"
#include "fgui_font.h"

#define FGUI_BUTTON_BG_COLOR 0xcccccccc
#define FGUI_BUTTON_BORDER_COLOR 0xaaaaaaaa
#define FGUI_BUTTON_TEXT_COLOR 0

void fgui_button_init(struct fgui_button *button, uint16_t x, uint16_t y,
		uint16_t w, uint16_t h, const char *text)
{
	button->x = x;
	button->y = y;
	button->width = w;
	button->height = h;
	strncpy(button->text, text, sizeof button->text);
	button->text[sizeof button->text - 1] = '\0';
}

void fgui_button_draw(struct fgui_button *button)
{
	/* button background */
	fgui_fill_rectangle(button->x, button->y, button->width,
			button->height, FGUI_BUTTON_BG_COLOR);
	/* border */
	fgui_draw_rectangle(button->x, button->y, button->width,
			button->height, FGUI_BUTTON_BORDER_COLOR);
	/* button text */
	fgui_draw_string(button->text, button->x+2, button->y+2,
			FGUI_BUTTON_TEXT_COLOR);
}
