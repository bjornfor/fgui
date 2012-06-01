#include <string.h>

#include "fgui_application.h"

void fgui_application_init(struct fgui_application *app)
{
	memset(app, 0, sizeof *app);
}

void fgui_application_process_event(struct fgui_application *app,
		struct fgui_event *event)
{
	/* TAB cycles focus */
	if (event->type == FGUI_EVENT_KEYDOWN && event->key.keycode == 0x09) { // 0x09 => TAB
		/* current widget loose focus */
		app->focus_widget->has_focus = false;
		if (app->focus_widget_idx + 1 < app->num_children) {
			app->focus_widget_idx++;
		} else {
			app->focus_widget_idx = 0;
		}
		/* next widget gain focus */
		app->focus_widget = app->children[app->focus_widget_idx];
		app->focus_widget->has_focus = true;
		return;
	}

	/* not tab, pass on to widget */
	if (!app->focus_widget) {
		return;
	}
	app->focus_widget->event_handler(app->focus_widget, event);
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
