#include "SDL.h"
#include "../src/fgui.h"

#define WIDTH 640
#define HEIGHT 480

#define LINEHEIGHT 20 /* pixels */

// Screen surface
SDL_Surface *gScreen;
struct fgui_application app;

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

// test some fgui primitives
void render_stuff(void)
{
	/* draw background */
	fgui_fill_rectangle(0, 0, WIDTH, HEIGHT, 0xdddddddd);

	fgui_draw_string("Use TAB to cycle focus", 50, 1*LINEHEIGHT, 0, NULL);

	fgui_draw_string("line:", 50, 2*LINEHEIGHT, 0, NULL);
	fgui_draw_line(200, 2*LINEHEIGHT, 220, 2*LINEHEIGHT, 0xff << 8);

	fgui_draw_string("empty rectangle", 50, 3*LINEHEIGHT, 0, NULL);
	fgui_draw_rectangle(200, 3*LINEHEIGHT, 10, 10, 0xff);

	fgui_draw_string("filled rectangle", 50, 4*LINEHEIGHT, 0, NULL);
	fgui_fill_rectangle(200, 4*LINEHEIGHT, 10, 10, 0xff << 8);

	fgui_draw_string("circle", 50, 5*LINEHEIGHT, 0, NULL);
	fgui_draw_circle(200, 5*LINEHEIGHT, 5, 0xffffffff);

	//fgui_draw_triangle(50, 50, 55, 55, 60, 50, 0xff);

	fgui_draw_string("this colored string\nspans multiple\nlines", 200, 6*LINEHEIGHT, 0xff << 16, NULL);

	fgui_draw_string("push button", 50, 8*LINEHEIGHT, 0, NULL);
	fgui_draw_string("2nd push button", 50, 9*LINEHEIGHT, 0, NULL);
	fgui_draw_string("label widget", 50, 11*LINEHEIGHT, 0, NULL);
	fgui_draw_string("combobox", 50, 10*LINEHEIGHT, 0, NULL);
	fgui_draw_string("lineedit", 50, 12*LINEHEIGHT, 0, NULL);

	fgui_application_draw(&app);
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

	sprintf(buf, "clicks: %d", ++(*data->number));
	printf("button %p clicked\n", data->button);
	fgui_button_set_text(data->button, buf);
}

void on_combobox_change(void *userdata)
{
	printf("%s\n", __func__);
}

enum fgui_key sdl_keysym_to_fgui_key(SDLKey sdl_keysym)
{
	enum fgui_key key;

	switch (sdl_keysym) {
		case SDLK_UP:    key = FGUI_KEY_ARROW_UP;    break;
		case SDLK_DOWN:  key = FGUI_KEY_ARROW_DOWN;  break;
		case SDLK_LEFT:  key = FGUI_KEY_ARROW_LEFT;  break;
		case SDLK_RIGHT: key = FGUI_KEY_ARROW_RIGHT; break;
		default:         key = sdl_keysym;           break;
	}

	return key;
}

int main(int argc, char *argv[])
{
	int ret;
	SDL_Event event;
	struct fgui_event fgui_event;
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
	fgui_button_init(&button, 200, 8*LINEHEIGHT, 82, 12, "hello world", NULL);
	fgui_button_init(&button2, 200, 9*LINEHEIGHT, 82, 12, "hello world 2", NULL);
	fgui_combobox_init(&combobox, 200, 10*LINEHEIGHT, 60, 12, NULL);
	fgui_label_init(&label, 200, 11*LINEHEIGHT, 150, 20, "hello fgui label", NULL);
	fgui_lineedit_init(&lineedit, 200, 12*LINEHEIGHT, 100, 15, NULL);
	fgui_combobox_add_item(&combobox, "item1");
	fgui_combobox_add_item(&combobox, "item2");
	fgui_combobox_add_item(&combobox, "item3");
	fgui_combobox_set_index(&combobox, 0);
	fgui_application_add_widget(&app, &button.base);
	fgui_application_add_widget(&app, &button2.base);
	fgui_application_add_widget(&app, (struct fgui_widget *)&label);
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
			fgui_event.key.keycode = sdl_keysym_to_fgui_key(event.key.keysym.sym);
			fgui_application_process_event(&app, &fgui_event);
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				// return (and thus, quit)
				return 0;
			default:
				fgui_event.type = FGUI_EVENT_KEYUP;
				fgui_event.key.keycode = sdl_keysym_to_fgui_key(event.key.keysym.sym);
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
