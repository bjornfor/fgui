#include "SDL.h"
#include "../src/fgui.h"

// Screen surface
SDL_Surface *gScreen;

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
	fgui_draw_line(0, 0, 10, 10, 0xff << 8);
	fgui_draw_rectangle(50, 100, 20, 30, 0xff);
	fgui_fill_rectangle(100, 100, 20, 30, 0xff << 8);
	fgui_draw_circle(300, 300, 100, 0xffffffff);
	fgui_fill_circle(200, 150, 20, 0xff << 16);
	fgui_draw_string("hello world", 100, 200, 0xff);
	fgui_draw_string("hello world", 100, 210, 0xff << 8);
	fgui_draw_string("hello world", 100, 220, 0xff << 16);
}

int main(int argc, char *argv[])
{
	int ret;
	SDL_Event event;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// make sure things are cleaned up when we quit
	atexit(SDL_Quit);

	// attempt to create a 640x480 window with 32bit pixels.
	gScreen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
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
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				// return (and thus, quit)
				return 0;
			default:
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
