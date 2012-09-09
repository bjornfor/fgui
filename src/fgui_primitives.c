/*
 * Copyright (c) 2012 Bjørn Forsman
 *
 * This file is part of fgui which is released under the MIT license. See
 * LICENSE.txt or go to http://www.opensource.org/licenses/MIT for full
 * license text.
 */

#include <stdint.h>
#include <stdlib.h>  // abs()
#include <stdbool.h>

#include "fgui_types.h"
#include "fgui_primitives.h"


/* Return true if point (x,y) is inside the given rectangle, else false. */
static bool is_in_rect(uint16_t x, uint16_t y, struct fgui_rect *r)
{
	return (x > r->x && x < (r->x + r->w) && y > r->y && y < (r->y + r->h));
}

void fgui_draw_point(uint16_t x, uint16_t y, uint32_t color, struct fgui_rect *clip)
{
	if (clip == NULL || is_in_rect(x, y, clip)) {
		fgui_set_pixel(x, y, color);
	}
}

/**
 * Bresenham's line algorithm:
 * http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 */
void fgui_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
		uint32_t color)
{
	int16_t dx, dy, sx, sy, err, e2;

	dx = abs(x1 - x0);
	dy = abs(y1 - y0);
	sx = x0 < x1 ? 1 : -1;
	sy = y0 < y1 ? 1 : -1;
	err = dx - dy;

	while (1) {
		fgui_set_pixel(x0, y0, color);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = 2 * err;
		if (e2 > -dy) {
			err = err - dy;
			x0 = x0 + sx;
		}
		if (e2 < dx) {
			err = err + dx;
			y0 = y0 + sy;
		}
	}
}

void fgui_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
		uint32_t color)
{
	uint16_t xpos, ypos;

	for (ypos = y; ypos < y + h; ypos++) {
		for (xpos = x; xpos < x + w; xpos++) {
			fgui_set_pixel(xpos, ypos, color);
		}
	}
}

void fgui_draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
		uint32_t color)
{
	fgui_fill_rectangle(x, y, w, 1, color);
	fgui_fill_rectangle(x, y, 1, h, color);
	fgui_fill_rectangle(x+w-1, y, 1, h, color);
	fgui_fill_rectangle(x, y+h-1, w, 1, color);
}

// The '(x != 0 && y != 0)' test in the last line of this function
// may be omitted for a performance benefit if the radius of the
// circle is known to be non-zero.
static void plot4points(uint16_t cx, uint16_t cy, uint16_t x, uint16_t y,
		uint32_t color)
{
	fgui_set_pixel(cx + x, cy + y, color);
	if (x != 0) fgui_set_pixel(cx - x, cy + y, color);
	if (y != 0) fgui_set_pixel(cx + x, cy - y, color);
	if (x != 0 && y != 0) fgui_set_pixel(cx - x, cy - y, color);
}
 
// used when drawing an (empty) circle
static void plot8points(uint16_t cx, uint16_t cy, uint16_t x, uint16_t y,
		uint32_t color)
{
	plot4points(cx, cy, x, y, color);
	if (x != y) plot4points(cx, cy, y, x, color);
}

// used when drawing a filled circle
static void draw4rectangles(uint16_t cx, uint16_t cy, uint16_t x, uint16_t y,
		uint32_t color)
{
	fgui_draw_rectangle(cx-y, cy-x, y<<1, 1, color);
	fgui_draw_rectangle(cx-x, cy-y, x<<1, 1, color);
	fgui_draw_rectangle(cx-x, cy+y, x<<1, 1, color);
	fgui_draw_rectangle(cx-y, cy+x, y<<1, 1, color);
}
 
/**
 * Midpoint circle algorithm:
 * http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 */
void fgui_draw_circle(uint16_t cx, uint16_t cy, uint16_t radius, uint32_t color)
{
	int16_t error = -radius;
	int16_t x = radius;
	int16_t y = 0;

	// The following while loop may altered to 'while (x > y)' for a
	// performance benefit, as long as a call to 'plot4points' follows
	// the body of the loop. This allows for the elimination of the
	// '(x != y)' test in 'plot8points', providing a further benefit.
	//
	// For the sake of clarity, this is not shown here.
	while (x >= y) {
		plot8points(cx, cy, x, y, color);

		error += y;
		y++;
		error += y;

		// The following test may be implemented in assembly language in
		// most machines by testing the carry flag after adding 'y' to
		// the value of 'error' in the previous step, since 'error'
		// nominally has a negative value.
		if (error >= 0) {
			error -= x;
			x--;
			error -= x;
		}
	}
}

/**
 * Midpoint (filled) circle algorithm.
 */
void fgui_fill_circle(uint16_t cx, uint16_t cy, uint16_t radius, uint32_t color)
{
	int16_t error = -radius;
	int16_t x = radius;
	int16_t y = 0;

	while (x >= y) {
		draw4rectangles(cx, cy, x, y, color);

		error += y;
		y++;
		error += y;

		if (error >= 0) {
			error -= x;
			x--;
			error -= x;
		}
	}
}

void fgui_draw_triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
		uint16_t x2, uint16_t y2, uint32_t color)
{
	fgui_draw_line(x0, y0, x1, y1, color);
	fgui_draw_line(x1, y1, x2, y2, color);
	fgui_draw_line(x2, y2, x0, y0, color);
}
