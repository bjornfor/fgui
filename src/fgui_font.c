/*
 * Copyright (c) 2012 Bjørn Forsman
 *
 * This file is part of fgui which is released under the MIT license. See
 * LICENSE.txt or go to http://www.opensource.org/licenses/MIT for full
 * license text.
 */

#include <stdint.h>
#include <stddef.h>

#include "fgui.h"
#include "fgui_font.h"
#include "fgui_font_data.h"
#include "utils.h"
#include "../font_experiments/output/font.h"


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

static int get_char_index2(wchar_t ch)
{
	int i;

	// look for char in font array
	for (i = 0; i < ARRAY_SIZE(cUnicode); i++) {
		if (cUnicode[i].codepoint == ch) {
			return i;
		}
	}
	return -1;
}

bool pixel_is_set2(unsigned char *xbmbits, uint16_t width, uint16_t height,
		uint16_t x, uint16_t y)
{
	uint16_t byteoffset;
	uint16_t bitoffset;

	/* round up width to match how the XBM data is laid out */
	width = (width + 7) & ~0x07;
	byteoffset = (y * width + x) / 8;
	bitoffset = x % 8;
	return xbmbits[byteoffset] & (1 << bitoffset);
}

static int draw_char2(wchar_t ch, uint16_t xpos, uint16_t ypos, uint32_t color)
{
	int i;
	int x;
	int y;
	int is_set;
	int width;
	int height;

	i = get_char_index2(ch);
	if (i < 0) {
		//printf("missing codepoint: %d\n", ch);
		return -1;
	}

	width = cUnicode[i].width;
	height = cUnicode[i].height;
	//printf("width: %d, height: %d\n", width, height);

	//fgui_draw_rectangle(xpos, ypos, width, height, 0xff);

	// draw character
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			is_set = pixel_is_set2(cUnicode[i].data, width, height, x, y);
			if (is_set) {
				fgui_set_pixel(xpos+x, ypos+y, color);
			}
		}
	}

	return 0;
}

void fgui_draw_string(const char *str, const uint16_t x, const uint16_t y, uint32_t color)
{
	int i;
	int j;
	int column;
	int line;
	int char_width;
	int char_height;

	int xoff = 0;
	int yoff = 0;

#if USE_NEW_FONT
	/* nop */
#else
	// assume all chars have the same width (monospace)
	char_width = cWidth[0];
	char_height = cHeight[0];
#endif

	column = 0;
	line = 0;
	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] == '\n') {
			line++;
			yoff += cUnicode[0].height; // height is constant
			xoff = 0; // height is constant
			column = 0;
			continue;
		}

#if USE_NEW_FONT
		j = get_char_index2(str[i]);
		if (j < 0) {
			//printf("missing codepoint: %d\n", ch);
			continue;
		}
		char_width = cUnicode[j].width;
		char_height = cUnicode[j].height;
		draw_char2(str[i], x + xoff, y + yoff, color);
		xoff += char_width - 2;
#else
		draw_char(str[i], x + column*char_width, y + line*char_height, color);
#endif
		column++;
	}
}
