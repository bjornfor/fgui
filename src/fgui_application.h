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
 * fgui application structure
 *
 * Every fgui program (that wants to use widgets that accept user input) needs
 * to create exactly one application structure. The application structure is
 * the parent of all widgets and knows what widget has focus. It also
 * dispatches input events to the widget that has focus.
 */

#ifndef FGUI_APPLICATION_H
#define FGUI_APPLICATION_H

#include <stddef.h>

#include "fgui_widget.h"

struct fgui_application {
	struct fgui_widget *focus_widget;

	int focus_widget_idx;
	// TODO: linked list?
	struct fgui_widget *children[FGUI_MAX_CHILDREN];

	size_t num_children;
};


void fgui_application_init(struct fgui_application *app);
void fgui_application_process_event(struct fgui_application *app, struct fgui_event *event);
int fgui_application_add_widget(struct fgui_application *app, struct fgui_widget *widget);

#endif /* FGUI_APPLICATION_H */
