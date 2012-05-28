#include <stdint.h>

#include "fgui.h"
#include "fgui_font.h"
#include "fgui_font_data.h"
#include "utils.h"


/** get the index of character 'ch' in the cAscii array */
static int get_char_index(char ch)
{
	int i;

	// look for char in font array
	for (i = 0; i < ARRAY_SIZE(cAscii); i++) {
		if (cAscii[i] == ch) {
			return i;
		}
	}
	return -1;
}

/** draw a single character */
static int draw_char(char ch, uint16_t xpos, uint16_t ypos, uint32_t color)
{
	int i;
	int x;
	int y;
	int pixel_is_set;

	i = get_char_index(ch);
	if (i < 0) {
		return -1;
	}

	// draw character
	for (y = 0; y < cHeight[i]; y++) {
		for (x = 0; x < cWidth[i]; x++) {
			pixel_is_set = (cData[cOff0[i] + y] & (1<<(cWidth[i]-x)));
			if (pixel_is_set) {
				fgui_set_pixel(xpos+x, ypos+y, color);
			}
		}
	}

	return 0;
}

void fgui_draw_string(const char *str, uint16_t x, uint16_t y, uint32_t color)
{
	int i;
	int column;
	int line;
	int char_width;
	int char_height;

	// assume all chars have the same width (monospace)
	char_width = cWidth[0];
	char_height = cHeight[0];
	column = 0;
	line = 0;
	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] == '\n') {
			line++;
			column = 0;
			continue;
		}
		draw_char(str[i], x + column*char_width, y + line*char_height, color);
		column++;
	}
}
