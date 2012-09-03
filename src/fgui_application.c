/*
 * Copyright (c) 2012 Bjørn Forsman
 *
 * This file is part of fgui which is released under the MIT license. See
 * LICENSE.txt or go to http://www.opensource.org/licenses/MIT for full
 * license text.
 */

#include <string.h>

#include "fgui_event.h"
#include "fgui_widget.h"
#include "fgui_application.h"

void fgui_application_init(struct fgui_application *app)
{
	memset(app, 0, sizeof *app);
}

static void set_next_focus_widget(struct fgui_application *app)
{
	int i;
	int widget_idx;
	struct fgui_widget *widget;

	/* current widget loose focus */
	app->focus_widget->has_focus = false;

	/* try the next widget until we find one that accepts focus */
	for (i = 0; i < app->num_children; i++) {
		widget_idx = (app->focus_widget_idx + i + 1) % app->num_children;
		widget = app->children[widget_idx];
		if (widget->focus_policy == FGUI_TAB_FOCUS) {
			app->focus_widget = widget;
			app->focus_widget_idx = widget_idx;
			app->focus_widget->has_focus = true;
			break;
		}
	}

	return;
}

void fgui_application_process_event(struct fgui_application *app,
		struct fgui_event *event)
{
	int ret;
	struct fgui_widget *widget;

	/* TAB cycles focus */
	if (event->type == FGUI_EVENT_KEYDOWN && event->key.keycode == FGUI_KEY_TAB) {
		set_next_focus_widget(app);
	}

	/* not tab, pass on to widget */
	if (!app->focus_widget) {
		return;
	}

	/*
	 * Send the event to each widget in the parent-child chain, until
	 * someone handles it.
	 */
	for (widget = app->focus_widget; widget != NULL; widget = widget->parent) {
		ret = widget->event_handler(widget, event);
		if (ret == 0) {
			break;
		}
	}
}

int fgui_application_add_widget(struct fgui_application *app, struct fgui_widget *widget)
{
	if (app->num_children >= FGUI_MAX_CHILDREN) {
		return -1;
	}

	if (app->num_children == 0) {
		app->focus_widget_idx = 0;
		app->focus_widget = widget;
		app->focus_widget->has_focus = true;
	}
	app->children[app->num_children++] = widget;
	return 0;
}

void fgui_application_draw(struct fgui_application *app)
{
	int i;

	for (i = 0; i < app->num_children; i++) {
		fgui_widget_draw(app->children[i]);
	}
}
