#include "SDL.h"
#include "../src/fgui.h"

#define WIDTH 640
#define HEIGHT 480

// Screen surface
SDL_Surface *gScreen;

struct fgui_button button;
struct fgui_button button2;
struct fgui_label label;
struct fgui_combobox combobox;
struct fgui_lineedit lineedit;

// define the "callback" that fgui uses to set pixels
void fgui_set_pixel(uint16_t x, uint16_t y, uint32_t color)
{
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = 1;
	r.h = 1;
	SDL_FillRect(gScreen, &r, color);
}

#include "../font_experiments/a_gimp.xbm"
#include "../font_experiments/output/codepoint_65.xbm"

bool pixel_is_set(unsigned char *xbmbits, uint16_t width, uint16_t height,
		uint16_t x, uint16_t y)
{
	uint16_t byteoffset;
	uint16_t bitoffset;

	/* round up width to match how the XBM data is laid out */
	width = (width + 7) & ~0x07;
	byteoffset = (y * width + x) / 8;
	bitoffset = x % 8;
	return xbmbits[byteoffset] & (1 << bitoffset);
}

/* draw the contents of a .xbm file */
#define DRAW_XBM(x, y, basename) \
	do { \
		int _x, _y; \
		\
		for (_y = 0; _y < basename##_height; _y++) { \
			for (_x = 0; _x < basename##_width; _x++) { \
				if (pixel_is_set(basename##_bits, basename##_width, basename##_height, _x, _y)) { \
					fgui_draw_point(x+_x, y+_y, 0xff); \
				} \
			} \
		} \
	} while (0)

// test some fgui primitives
void render_stuff(void)
{
	/* draw background */
	fgui_fill_rectangle(0, 0, WIDTH, HEIGHT, 0xdddddddd);

	//DRAW_XBM(0, 0, a_gimp);
	//DRAW_XBM(100, 10, a);
	//fgui_draw_string("C", 0, 0, 0);
	//fgui_draw_rectangle(0, 0, 20, 25, 0xff);
	//fgui_draw_string("Use TAB to cycle focus", 50, 20, 0);
#if 1
	fgui_draw_string("Use TAB to cycle focus", 50, 20, 0);

	fgui_draw_string("line:", 50, 50, 0);
	fgui_draw_line(200, 50, 220, 50, 0xff << 8);

	fgui_draw_string("empty rectangle", 50, 70, 0);
	fgui_draw_rectangle(200, 70, 10, 10, 0xff);

	fgui_draw_string("filled rectangle", 50, 90, 0);
	fgui_fill_rectangle(200, 90, 10, 10, 0xff << 8);

	fgui_draw_string("filled circle", 50, 110, 0);
	fgui_draw_circle(200, 110, 5, 0xffffffff);

	//fgui_draw_triangle(50, 50, 55, 55, 60, 50, 0xff);

	fgui_draw_string("label widget", 50, 230, 0);
	fgui_label_draw((struct fgui_widget *)&label);

	fgui_draw_string("this colored string\nspans multiple\nlines", 200, 120, 0xff << 16);

	fgui_draw_string("push button", 50, 170, 0);
	fgui_button_draw((struct fgui_widget *)&button);

	fgui_draw_string("2nd push button", 50, 190, 0);
	fgui_button_draw((struct fgui_widget *)&button2);

	fgui_draw_string("combobox", 50, 210, 0);
	fgui_widget_draw((struct fgui_widget *)&combobox);

	fgui_draw_string("lineedit", 50, 250, 0);
	fgui_widget_draw((struct fgui_widget *)&lineedit);
#endif
}

struct btn_cb_data {
	struct fgui_button *button;
	char *str;
	int *number;
};

void on_button_click(void *arg)
{
	char buf[30];
	struct btn_cb_data *data = arg;

	sprintf(buf, "clicked \n%d times", ++(*data->number));
	printf("button %p clicked\n", data->button);
	fgui_button_set_text(data->button, buf);
}

void on_combobox_change(void *userdata)
{
	printf("%s\n", __func__);
}

int main(int argc, char *argv[])
{
	int ret;
	SDL_Event event;
	struct fgui_event fgui_event;
	struct fgui_application app;
	int button_clicks = 0;
	int button2_clicks = 0;
	struct btn_cb_data btn_cb_data = {
		&button,
		"hello button userdata",
		&button_clicks,
	};

	struct btn_cb_data btn_cb_data2 = {
		&button2,
		"hello button2 userdata",
		&button2_clicks,
	};

	fgui_application_init(&app);
	fgui_button_init(&button, 200, 170, 82, 12, "hello world", NULL);
	fgui_button_init(&button2, 200, 190, 82, 12, "hello world 2", NULL);
	fgui_label_init(&label, 200, 230, "hello fgui label", NULL);
	fgui_lineedit_init(&lineedit, 200, 250, 100, 15, NULL);
	fgui_combobox_init(&combobox, 200, 210, 60, 12, NULL);
	fgui_combobox_add_item(&combobox, "item1");
	fgui_combobox_add_item(&combobox, "item2");
	fgui_combobox_add_item(&combobox, "item3");
	fgui_combobox_set_index(&combobox, 0);
	fgui_application_add_widget(&app, &button.base);
	fgui_application_add_widget(&app, &button2.base);
	fgui_application_add_widget(&app, (struct fgui_widget *)&combobox);
	fgui_application_add_widget(&app, (struct fgui_widget *)&lineedit);
	fgui_button_set_on_click_handler(&button, on_button_click, &btn_cb_data);
	fgui_button_set_on_click_handler(&button2, on_button_click, &btn_cb_data2);
	fgui_combobox_set_on_change_handler(&combobox, on_combobox_change, NULL);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// make sure things are cleaned up when we quit
	atexit(SDL_Quit);

	// attempt to create a 640x480 window with 32bit pixels.
	gScreen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_SWSURFACE);
	if (gScreen == NULL) {
		fprintf(stderr, "Unable to set up video: %s\n", SDL_GetError());
		exit(1);
	}

	while (1) {
		render_stuff();

		// update the whole screen
		SDL_UpdateRect(gScreen, 0, 0, 0, 0);

		// blocking wait (tip: SDL_PollEvent is non-blocking)
		ret = SDL_WaitEvent(&event);
		if (ret == 0) {
			fprintf(stderr, "ERROR: SDL_WaitEvent: %s\n", SDL_GetError());
			exit(1);
		}

		switch (event.type) {
		case SDL_KEYDOWN:
			fgui_event.type = FGUI_EVENT_KEYDOWN;
			fgui_event.key.keycode = event.key.keysym.sym;
			fgui_application_process_event(&app, &fgui_event);
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				// return (and thus, quit)
				return 0;
			default:
				fgui_event.type = FGUI_EVENT_KEYUP;
				fgui_event.key.keycode = event.key.keysym.sym;
				fgui_application_process_event(&app, &fgui_event);
				break;
			}
			break;

		case SDL_QUIT:
			return 0;

		default:
			break;
		}
	}
	return 0;
}
