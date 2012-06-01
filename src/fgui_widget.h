/**
 * @file
 *
 * fgui base widget structure
 */

#ifndef FGUI_WIDGET_H
#define FGUI_WIDGET_H

#include <stdint.h>
#include <stddef.h>   // size_t
#include <stdbool.h>


#define FGUI_MAX_CHILDREN 10


enum fgui_event_type {
	FGUI_EVENT_KEYDOWN,
	FGUI_EVENT_KEYUP,
};

struct fgui_keyboard_event {
	unsigned char keycode;
};

struct fgui_event {
	enum fgui_event_type type;
	union {
		struct fgui_keyboard_event key;
		// extend with other event types here
	};
};

enum fgui_focus_policy {
	FGUI_NO_FOCUS,  //< widget do not accept focus (e.g. label widgets)
	FGUI_TAB_FOCUS, //< widget accepts focus by tabbing
};

/**
 * Base fgui widget
 */
struct fgui_widget {
	/**
	 * Horisontal position of widget.
	 *
	 * For labels and rectangles it's the upper left corner. For circles
	 * it's the center.
	 */
	uint16_t x;

	/**
	 * Vertical position of widget.
	 *
	 * For labels and rectangles it's the upper left corner. For circles
	 * it's the center.
	 */
	uint16_t y;

	/** does this widget support focus? */
	enum fgui_focus_policy focus_policy;

	/**
	 * widget has input focus
	 *
	 * TODO: maybe it's better to let a global application struct keep a
	 * pointer to the currently focused widget? We also need to define a
	 * a way to traverse the focus chain...
	 */
	bool has_focus;

	/**
	 * Enabled widgets accept focus, disabled do not. Disabled widgets may
	 * display themselves differently (e.g. greyed out buttons).
	 */
	bool is_enabled;

	/** parent widget or NULL */
	struct fgui_widget *parent;

	// TODO: linked list?
	struct fgui_widget *children[FGUI_MAX_CHILDREN];

	size_t num_children;

	/** draw widget, must be non-NULL */
	void (*draw)(struct fgui_widget *widget);

	/**
	 * Function that handles input events, must be non-NULL.
	 *
	 * If an event handler accepts the event it receives it should return
	 * 0. If it doesn't accept it should return non-zero and the event will
	 * bubble up in the parent chain. If the top level widget also discards
	 * the event it is silently dropped.
	 */
	int (*event_handler)(struct fgui_widget *widget, struct fgui_event *event);
};


void fgui_widget_init(struct fgui_widget *widget, struct fgui_widget *parent);
void fgui_widget_set_draw(struct fgui_widget *widget, void (*draw)(struct fgui_widget *widget));
int fgui_widget_add_child(struct fgui_widget *widget, struct fgui_widget *child);

#endif /* FGUI_WIDGET_H */
