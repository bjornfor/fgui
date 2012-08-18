/*
 * Copyright (c) 2012 Bjørn Forsman
 *
 * This file is part of fgui which is released under the MIT license. See
 * LICENSE.txt or go to http://www.opensource.org/licenses/MIT for full
 * license text.
 */

#ifndef FGUI_INPUT_METHOD_H
#define FGUI_INPUT_METHOD_H

#include <stdint.h>
#include <string.h>

#include "fgui_widget.h"

struct fgui_input_method {
	/** last *relevant* event */
	struct fgui_event last_event;
	char *preedit;
};

/**
 * @param im Pointer to an fgui_input_method struct
 * @param event Pointer to the event to process
 * @param input_widget (Input) widget that will receive "preedit" and "commit"
 * events.
 */
void fgui_input_method_process_event(struct fgui_input_method *im,
		struct fgui_event *event, struct fgui_widget *input_widget);

#endif /* FGUI_INPUT_METHOD_H */
