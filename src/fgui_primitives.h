#ifndef FGUI_PRIMITIVES_H
#define FGUI_PRIMITIVES_H

#include <stdint.h>


/**
 * Draw a point at (x, y).
 */
void fgui_draw_point(uint16_t x, uint16_t y, uint32_t color);

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


#endif /* FGUI_PRIMITIVES_H */
