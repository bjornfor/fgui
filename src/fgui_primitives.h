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
 * fgui drawing primitives
 */

#ifndef FGUI_PRIMITIVES_H
#define FGUI_PRIMITIVES_H

#include <stdint.h>

#include "fgui_types.h"


#define FGUI_COLOR(r, g, b) (((r << 16) & 0xff0000) | ((g << 8) & 0xff00) | (b & 0xff))

/**
 * This is the "callback" that fgui uses when it needs to draw something on the
 * screen. It must be defined by the application.
 *
 * @param x Pixel position in the x direction.
 * @param y Pixel position in the y direction.
 * @param color Color of the pixel. The MSB is ignored and the next three less
 * significant bytes are the R, G and B color values.
 */
void fgui_set_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * Draw a point at (x, y).
 *
 * @param x     x coordinate
 * @param y     y coordinate
 * @param color @see fgui_set_pixel()
 * @param clip  Pointer to a clipping rectangle. The point is drawn only if it
 * is inside the rectangle. Set to NULL to disable clipping.
 */
void fgui_draw_point(uint16_t x, uint16_t y, uint32_t color, struct fgui_rect *clip);

/**
 * Draw a line from (x0, y0) to (x1, y1).
 */
void fgui_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
		uint32_t color);

/**
 * Draw a filled rectangle.
 *
 * @param x The x coordinate of the upper left corner.
 * @param y The y coordinate of the upper left corner.
 * @param w Width of rectangle.
 * @param h Height of rectangle.
 */
void fgui_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
		uint32_t color);

/**
 * Draw a rectangle outline.
 *
 * @param x The x coordinate of the upper left corner.
 * @param y The y coordinate of the upper left corner.
 * @param w Width of rectangle.
 * @param h Height of rectangle.
 */
void fgui_draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
		uint32_t color);

/**
 * Draw a circle of a given radius with center at (cx, cy).
 */
void fgui_draw_circle(uint16_t cx, uint16_t cy, uint16_t radius, uint32_t color);

/**
 * Fill a circle of a given radius with center at (cx, cy).
 */
void fgui_fill_circle(uint16_t cx, uint16_t cy, uint16_t radius, uint32_t color);

/**
 * Draw a triangle with corners at (x0,y0), (x1,y1), (x2,y2).
 */
void fgui_draw_triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
		uint16_t x2, uint16_t y2, uint32_t color);


#endif /* FGUI_PRIMITIVES_H */
