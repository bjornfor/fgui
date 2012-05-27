#ifndef FGUI_LABEL_H
#define FGUI_LABEL_H

#define MAX_TEXT_LEN 50

struct fgui_label {
	uint16_t x;
	uint16_t y;
	char text[MAX_TEXT_LEN];
};

void fgui_label_init(struct fgui_label *label, uint16_t x, uint16_t y, const
		char *text);
void fgui_label_set_text(struct fgui_label *label, char *text);
void fgui_label_draw(struct fgui_label *label);

#endif /* FGUI_LABEL_H */
