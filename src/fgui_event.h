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
 * fgui event types.
 */

#ifndef FGUI_EVENT_H
#define FGUI_EVENT_H

enum fgui_event_type {
	FGUI_EVENT_KEYDOWN,
	FGUI_EVENT_KEYUP,
};

struct fgui_keyboard_event {
	unsigned int keycode;
};

struct fgui_event {
	enum fgui_event_type type;
	union {
		struct fgui_keyboard_event key;
		// extend with other event types here
	};
};

#endif /* FGUI_EVENT_H */
