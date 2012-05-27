/**
 * @file
 *
 * fgui button module
 */

#ifndef FGUI_BUTTON_H
#define FGUI_BUTTON_H

#define MAX_TEXT_LEN 50

struct fgui_button {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	char text[MAX_TEXT_LEN];
};

void fgui_button_init(struct fgui_button *button, uint16_t x, uint16_t y,
		uint16_t w, uint16_t h, const char *text);
void fgui_button_draw(struct fgui_button *button);

#endif /* FGUI_BUTTON_H */
