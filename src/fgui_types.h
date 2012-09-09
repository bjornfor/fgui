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
 * General datatypes.
 */

#ifndef FGUI_TYPES_H
#define FGUI_TYPES_H

/**
 * A rectangle.
 */
struct fgui_rect {
	uint16_t x; /**< x coord of upper left corner */
	uint16_t y; /**< y coord of upper left corner */
	uint16_t w; /**< width of rectangle */
	uint16_t h; /**< height of rectangle */
};

#endif /* FGUI_TYPES_H */
