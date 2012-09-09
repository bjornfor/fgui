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
 * fgui font/string drawing functions
 */

#ifndef FGUI_FONT_H
#define FGUI_FONT_H

#include <stdint.h>

#include "fgui_types.h"

/**
 * Draw a string starting at (x, y).
 *
 * (x, y) is the upper left corner of the string.
 */
void fgui_draw_string(const char *str, uint16_t x, uint16_t y, uint32_t color,
		struct fgui_rect *clip);

#endif /* FGUI_FONT_H */
