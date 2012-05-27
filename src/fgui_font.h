#ifndef FGUI_FONT_H
#define FGUI_FONT_H

#include <stdint.h>

/**
 * Draw a string starting at (x, y).
 *
 * (x, y) is the upper left corner of the string.
 */
void fgui_draw_string(const char *str, uint16_t x, uint16_t y, uint32_t color);

#endif /* FGUI_FONT_H */
