/**
 * @file
 *
 * Top-level header file for fgui.
 */

#ifndef FGUI_H
#define FGUI_H

#include <stdint.h>

#include "fgui_primitives.h"
#include "fgui_font.h"
#include "fgui_button.h"


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


#endif /* FGUI_H */
