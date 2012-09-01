/*
 * Copyright (c) 2012 Bjørn Forsman
 *
 * This file is part of fgui which is released under the MIT license. See
 * LICENSE.txt or go to http://www.opensource.org/licenses/MIT for full
 * license text.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "fgui_event.h"
#include "fgui_widget.h"


static void null_draw(struct fgui_widget *widget)
{
	/* empty */
}

static int null_event_handler(struct fgui_widget *widget, struct fgui_event *event)
{
	/* we didn't handle this event */
	return 1;
}

int fgui_widget_init(struct fgui_widget *widget, struct fgui_widget *parent)
{
	int ret = 0;

	memset(widget, 0, sizeof *widget);
	widget->draw = null_draw;
	widget->event_handler = null_event_handler;
	widget->parent = parent;
	if (parent) {
		ret = fgui_widget_add_child(parent, widget);
	}
	widget->is_enabled = true;
	widget->focus_policy = FGUI_TAB_FOCUS;
	return ret;
}

void fgui_widget_set_draw(struct fgui_widget *widget, void (*draw)(struct fgui_widget *widget))
{
	widget->draw = draw;
}

int fgui_widget_add_child(struct fgui_widget *widget, struct fgui_widget *child)
{
	if (widget->num_children >= FGUI_MAX_CHILDREN) {
		return -1;
	}

	widget->children[widget->num_children++] = child;
	return 0;
}

void fgui_widget_draw(struct fgui_widget *widget)
{
	widget->draw(widget);
}
