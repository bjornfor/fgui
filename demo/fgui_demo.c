#include <stdio.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#include "SDL.h"
#include "../src/fgui.h"
#include "../src/utils.h"
#include "../src/fgui_3d.h"

#define WIDTH 640
#define HEIGHT 480

#define LINEHEIGHT 20 /* pixels */

#define NUM_POINTS(box_wireframe) (sizeof(box_wireframe) / sizeof(box_wireframe[0]) * 2)


// Screen surface
SDL_Surface *gScreen;
struct fgui_application app;

struct fgui_button button;
struct fgui_button button2;
struct fgui_label label;
struct fgui_combobox combobox;
struct fgui_lineedit lineedit;

struct fgui_vec4 vec4;

/*
 * Some line coordinates in 3D
 *
 * Each index (row) is a 2 element array (begin/end points of a line) of each 4
 * (x,y,z,w) coordinates.
 */
int box_wireframe[][2][4] = {
	/* start_vec, end_vec */
	/* x0, y0, z0, w0   x1, y1, z1, w0 */
	/* front side (z=near) */
	{{0, 0, 0, 0},   {1, 0, 0, 0}},
	{{1, 0, 0, 0},   {1, 1, 0, 0}},
	{{1, 1, 0, 0},   {0, 1, 0, 0}},
	{{0, 1, 0, 0},   {0, 0, 0, 0}},

	/* z-axis lines (z near -> far) */
	{{0, 0, 0, 0},   {0, 0, 1, 0}},
	{{1, 0, 0, 0},   {1, 0, 1, 0}},
	{{1, 1, 0, 0},   {1, 1, 1, 0}},
	{{0, 1, 0, 0},   {0, 1, 1, 0}},

	/* back side (z=far) */
	{{0, 0, 1, 0},   {1, 0, 1, 0}},
	{{1, 0, 1, 0},   {1, 1, 1, 0}},
	{{1, 1, 1, 0},   {0, 1, 1, 0}},
	{{0, 1, 1, 0},   {0, 0, 1, 0}},
};

/* multiply two 4x4 matrices */
void mat_mult_44_44(float m1[4][4], float m2[4][4], float out[4][4])
{
	int i;
	int row, col;
	float elem;
	const int dim = 4;

	for (row = 0; row < dim; row++)
	{
		for (col = 0; col < dim; col++)
		{
			elem = 0.0;
			for (i = 0; i < dim; i++)
			{
				elem += m1[row][i] * m2[i][col];
			}
			out[row][col] = elem;
		}
	}
}

/* multiply a 4x4 matrix and 4x1 vector (result: 4x1 vector) */
void mat_mult_44_41(float m1[4][4], float vec[4], float out[4])
{
	out[0] = m1[0][0] * vec[0]
	       + m1[0][1] * vec[1]
	       + m1[0][2] * vec[2]
	       + m1[0][3] * vec[3];

	out[1] = m1[1][0] * vec[0]
	       + m1[1][1] * vec[1]
	       + m1[1][2] * vec[2]
	       + m1[1][3] * vec[3];

	out[2] = m1[2][0] * vec[0]
	       + m1[2][1] * vec[1]
	       + m1[2][2] * vec[2]
	       + m1[2][3] * vec[3];

	out[3] = m1[3][0] * vec[0]
	       + m1[3][1] * vec[1]
	       + m1[3][2] * vec[2]
	       + m1[3][3] * vec[3];
}

//void translate(float x, float y, float z, float m1[4][4], float out[4][4])
void translate(float x, float y, float z, int vec[4], int out[4])
{
	/* translation matrix */
	float tm[4][4] = {
		{1, 0, 0, x},
		{0, 1, 0, y},
		{0, 0, 1, z},
		{0, 0, 0, 1},
	};

	/* mat_mult(tm, m1, out); */

	out[0] = tm[0][0] * vec[0] +
	         tm[0][1] * vec[1] +
	         tm[0][2] * vec[2] +
	         tm[0][3] * vec[3];
	out[1] = tm[1][0] * vec[0] +
	         tm[1][1] * vec[1] +
	         tm[1][2] * vec[2] +
	         tm[1][3] * vec[3];
	out[2] = tm[2][0] * vec[0] +
	         tm[2][1] * vec[1] +
	         tm[2][2] * vec[2] +
	         tm[2][3] * vec[3];
	out[3] = tm[3][0] * vec[0] +
	         tm[3][1] * vec[1] +
	         tm[3][2] * vec[2] +
	         tm[3][3] * vec[3];
}

void translate2(float x, float y, float z, float vec[4], float out[4])
{
	/* translation matrix */
	float tm[4][4] = {
		{1, 0, 0, x},
		{0, 1, 0, y},
		{0, 0, 1, z},
		{0, 0, 0, 1},
	};

	mat_mult_44_41(tm, vec, out);
}

// Rotate angle (in degrees) around vector [x, y, z]
void rotate(float angle, float x, float y, float z, int vec[4], int out[4])
{
	// TODO: implement
	/* rotation matrix */
	float rm[4][4] = {
		{1, 0, 0, x},
		{0, 1, 0, y},
		{0, 0, 1, z},
		{0, 0, 0, 1},
	};
}


void scale(float x, float y, float z, int vec[4], int out[4])
{
	out[0] = vec[0] * x;
	out[1] = vec[1] * y;
	out[2] = vec[2] * z;
	out[3] = vec[3];
}

void normalize(int vec[4], int out[4])
{
	float len = sqrt((vec[0] * vec[0])
	               + (vec[1] * vec[1])
	               + (vec[2] * vec[2]));
	out[0] = vec[0] / len;
	out[1] = vec[1] / len;
	out[2] = vec[2] / len;
	out[3] = vec[3];
}


/* Project 3d point onto 2d plane */
void project(int x, int y, int z, int *xout, int *yout)
{
	/* distance from the eye position to the projection plane */
	float ez = 1.0;

	/* perspective projection */
	if (z == 0) {
		z = 1;
	}
	/* *xout = (x / ((float)z * 1.0)); */
	/* *yout = (y / ((float)z * 1.0)); */

	*xout = x * (ez / z);
	*yout = y * (ez / z);

	/* orthographic projection */
	/* *xout = x; */
	/* *yout = y; */
}

void draw_box3d(int box_wireframe[12][2][4], int num_lines)
{
	int i;
	int x0, x1, y0, y1;
	int out[4];
	int bwt[12][2][4] = {0}; /* box wireframe translated */

	for (i = 0; i < num_lines; i++)
	{
		/* x, y, z */
                translate(0, 0, 3, box_wireframe[i][0], bwt[i][0]);
                translate(0, 0, 3, box_wireframe[i][1], bwt[i][1]);

		project(bwt[i][0][0],
			bwt[i][0][1],
			bwt[i][0][2],
			&x0, &y0);
		project(bwt[i][1][0],
			bwt[i][1][1],
			bwt[i][1][2],
			&x1, &y1);

		printf("drawing line p0=(%2d,%2d), p1=(%2d,%2d)\n", x0, y0, x1, y1);
		fgui_draw_line(10+x0, 10+y0, 10+x1, 10+y1, FGUI_COLOR(255,0,0));
	}

}

/* translate and draw vertices */
void draw_vertices(const struct fgui_vec4 *vertices, int num_vertices)
{
	int i;
	int x, y;
	const int max_vertices = 32;
	struct fgui_vec4 translated_vertices[max_vertices];

	assert(num_vertices <= max_vertices);

	// Uhm, I could handle this with a single (translated) vertex variable,
	// instead of the array.

	for (i = 0; i < num_vertices; i++)
	{
		fgui_transform_vector(&vertices[i], 10,10,10, 0,0,0, 15,15,0, &translated_vertices[i]);
		project(translated_vertices[i].x,
			translated_vertices[i].y,
			translated_vertices[i].z,
			&x, &y);
		fgui_set_pixel(x, y, FGUI_COLOR(255, 0, 0));
	}
}

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
	/* fgui_draw_string("Use TAB to cycle focus", 50, 1*LINEHEIGHT, 0, NULL); */
	/* return; */

	/* draw background */
	fgui_fill_rectangle(0, 0, WIDTH, HEIGHT, FGUI_COLOR(221,221,221));

	fgui_draw_string("Use TAB to cycle focus", 50, 1*LINEHEIGHT, 0, NULL);

	fgui_draw_string("line:", 50, 2*LINEHEIGHT, FGUI_COLOR(0,0,0), NULL);
	fgui_draw_line(200, 2*LINEHEIGHT, 220, 2*LINEHEIGHT, FGUI_COLOR(0,255,0));

	fgui_draw_string("empty rectangle", 50, 3*LINEHEIGHT, FGUI_COLOR(0,0,0), NULL);
	fgui_draw_rectangle(200, 3*LINEHEIGHT, 10, 10, FGUI_COLOR(0,0,255));

	fgui_draw_string("filled rectangle", 50, 4*LINEHEIGHT, 0, NULL);
	fgui_fill_rectangle(200, 4*LINEHEIGHT, 10, 10, FGUI_COLOR(0, 255, 0));

	fgui_draw_string("circle", 50, 5*LINEHEIGHT, FGUI_COLOR(0,0,0), NULL);
	fgui_draw_circle(200, 5*LINEHEIGHT, 5, FGUI_COLOR(255, 255, 255));

	//fgui_draw_triangle(50, 50, 55, 55, 60, 50, 0xff);

	fgui_draw_string("this colored string\nspans multiple\nlines", 200, 6*LINEHEIGHT,
		FGUI_COLOR(255,0,0), NULL);

	fgui_draw_string("push button", 50, 8*LINEHEIGHT, FGUI_COLOR(0,0,0), NULL);
	fgui_draw_string("2nd push button", 50, 9*LINEHEIGHT, FGUI_COLOR(0,0,0), NULL);
	fgui_draw_string("label widget", 50, 11*LINEHEIGHT, FGUI_COLOR(0,0,0), NULL);
	fgui_draw_string("combobox", 50, 10*LINEHEIGHT, FGUI_COLOR(0,0,0), NULL);
	fgui_draw_string("lineedit", 50, 12*LINEHEIGHT, FGUI_COLOR(0,0,0), NULL);

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
	int a[] = {cos(1)};
	int ret;
	int i;
	int j;
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
	bool need_redraw = true;
	struct timeval tv0, tv1;
	struct timespec ts0, ts1, res;
	int usec;

	if (argc > 1) {
		/* for "perf stat ./fgui_demo" */
		for (i = 0; i < 1000; i++)
		{
			render_stuff();
		}
		return 0;
	}

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

	clockid_t clkid = CLOCK_THREAD_CPUTIME_ID; //CLOCK_PROCESS_CPUTIME_ID;
	ret = clock_getres(clkid, &res);
	if (ret != 0) {
		perror("clock_getres");
		return 1;
	}

	printf("clock_gettime resolution: %ds%0dns\n", res.tv_sec, res.tv_nsec);

	printf("NUM_POINTS: %d\n", NUM_POINTS(box_wireframe));

	/* scale up the box */
	for (i = 0; i < NUM_POINTS(box_wireframe); i++)
	{
		scale(30, 30, 30, box_wireframe[i][0], box_wireframe[i][0]);
		scale(30, 30, 30, box_wireframe[i][1], box_wireframe[i][1]);
	}

	while (1) {

		if (need_redraw) {
			clock_gettime(clkid, &ts0);
			for (i = 0; i < 1; i++) {
				/* render_stuff(); */

				/* draw background */
				fgui_fill_rectangle(0, 0, WIDTH, HEIGHT, FGUI_COLOR(221,221,221));
				//draw_box3d(box_wireframe, NUM_POINTS(box_wireframe) / 2);
				draw_vertices(box_vertices, ARRAY_SIZE(box_vertices));
			}
			clock_gettime(clkid, &ts1);

			usec = ((ts1.tv_nsec - ts0.tv_nsec) / 1000);
			if (usec < 0) {
				usec += 1000000;
			}

			printf("clock_gettime: %d us\n", usec);

			// update the whole screen
			SDL_UpdateRect(gScreen, 0, 0, 0, 0);
		}

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
			need_redraw = true;
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
				need_redraw = true;
				break;
			}
			break;

		case SDL_QUIT:
			return 0;

		default:
			need_redraw = false;
			break;
		}
	}
	return 0;
}
