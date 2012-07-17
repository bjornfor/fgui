/*
 * Take a font file and a character and write XBM (X bitmap) data to stdout.
 *
 * Dependencies: freetype2
 *
 * TODO: implement
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <stdint.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define USE_MONOCHROME 0

#define WIDTH  80
#define HEIGHT 16

/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];

/* Replace this function with something useful. */

void draw_bitmap(FT_Bitmap *bitmap, FT_Int x, FT_Int y)
{
	FT_Int i, j, p, q;
	FT_Int x_max = x + bitmap->width;
	FT_Int y_max = y + bitmap->rows;

#if USE_MONOCHROME
#else
	for (i = x, p = 0; i < x_max; i++, p++) {
		for (j = y, q = 0; j < y_max; j++, q++) {
			if (i < 0 || j < 0 || i >= WIDTH || j >= HEIGHT)
				continue;

			image[j][i] |= bitmap->buffer[q * bitmap->width + p];
		}
	}
#endif
}

void set_pixel(unsigned char *xbmbits, uint16_t width, uint16_t height,
		uint16_t x, uint16_t y)
{
	uint16_t byteoffset;
	uint16_t bitoffset;

	/* round up width to match how the XBM data is laid out */
	width = (width + 7) & ~0x07;
	byteoffset = (y * width + x) / 8;
	bitoffset = x % 8;
	xbmbits[byteoffset] |= (1 << bitoffset);
}

// TODO: use wchar_t, long int or uint32_t?
void dump_as_xbm(FT_GlyphSlot slot, char ch)
{
	int i;
	FT_Int x, y;
	unsigned char xbmbits[100] = {0};
	int rows = slot->bitmap.rows;
	int width = slot->bitmap.width;
	int pitch = slot->bitmap.pitch;

	printf("bitmap stuff: width: %d, rows: %d, pitch: %d, num_grays: %d\n"
			, slot->bitmap.width
			, slot->bitmap.rows
			, slot->bitmap.pitch
			, slot->bitmap.num_grays
			);

	printf("glyph->metrics stuff: width: %ld, height: %ld, horiBearingX: %ld, horiBearingY: %ld, horiAdvance: %ld\n"
			, slot->metrics.width >> 6
			, slot->metrics.height >> 6
			, slot->metrics.horiBearingX >> 6
			, slot->metrics.horiBearingY >> 6
			, slot->metrics.horiAdvance >> 6
			);


	FILE *fp = fopen("tmp.xbm", "w");
	if (!fp) {
		printf("error opening tmp.xbm: %s\n", strerror(errno));
		exit(1);
	}
	fprintf(fp, "#define codepoint_%d_height %ld\n", (int)ch, slot->metrics.height >> 6);
	fprintf(fp, "#define codepoint_%d_width %ld\n", (int)ch, slot->metrics.width >> 6);
	fprintf(fp, "static unsigned char codepoint_%d_bits = {\n", (int)ch);

	for (y = 0; y < rows; y++) {
		for (x = 0; x < pitch; x++) {
			if (slot->bitmap.buffer[y * pitch + x] > 90) {
				set_pixel(xbmbits, pitch, rows, x, y);
			}
		}
	}

	// TODO: don't write more bytes than the .xbm header defines
	// (width_rounded_up * height_rounded_up)
	for (i = 0; i < sizeof xbmbits; i++) {
		fprintf(fp, "0x%02x, ", xbmbits[i]);
	}
	fprintf(fp, "\n};\n");
	fclose(fp);
}

void show_image(void)
{
	int i, j;

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			/* putchar( image[i][j] == 0 ? ' ' : image[i][j] < 128 ? '+' : '*' ); */
			putchar(image[i][j] > 90 ? '*' : ' ');
			/* printf("%3x, ", image[i][j] ); */
		}
		putchar('\n');
	}
}

// TODO: implement
void show_image_monochrome(void)
{
	int i, j;

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++)
			/* putchar( image[i][j] == 0 ? ' ' : image[i][j] < 128 ? '+' : '*' ); */
			/* putchar( image[i][j] > 128 ? '*' : ' ' ); */
			printf("%3x, ", image[i][j]);
		putchar('\n');
	}
}

int main(int argc, char **argv)
{
	FT_Library library;
	FT_Face face;

	FT_GlyphSlot slot;
	FT_Matrix matrix;	/* transformation matrix */
	FT_Vector pen;		/* untransformed origin  */
	FT_Error error;
	//FT_Bitmap bitmap;
	//FT_Pos width;
	//FT_Pos rows;
	//FT_Pos pitch;

	char *fontfilename;
	char *text;

	double angle;
	int target_height;
	int n, num_chars;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s fontfile character\n", argv[0]);
		exit(1);
	}

	fontfilename = argv[1];	/* first argument */
	text = argv[2];		/* second argument */
	num_chars = strlen(text);
	angle = (0 / 360.0) * 3.14159 * 2;	/* use N degrees */
	target_height = HEIGHT;

	error = FT_Init_FreeType(&library);	/* initialize library */
	if (error) {
		printf("error: %d\n", error);
		exit(1);
	}

	error = FT_New_Face(library, fontfilename, 0, &face);	/* create face object */
	if (error) {
		printf("error: %d\n", error);
		exit(1);
	}

	/* bfo: set character sizes directly in pixels, better for embedded */
	error = FT_Set_Pixel_Sizes(face, 0 /* width */, 12 /* height */);
	if (error) {
		printf("error: %d\n", error);
		exit(1);
	}

	slot = face->glyph;

	/* set up matrix */
	matrix.xx = (FT_Fixed) (cos(angle) * 0x10000L);
	matrix.xy = (FT_Fixed) (-sin(angle) * 0x10000L);
	matrix.yx = (FT_Fixed) (sin(angle) * 0x10000L);
	matrix.yy = (FT_Fixed) (cos(angle) * 0x10000L);

	/* the pen position in 26.6 cartesian space coordinates; */
	/* start at (300,200) relative to the upper left corner  */
	pen.x = 0 * 64;
	pen.y = (target_height - HEIGHT*0.7) * 64;

	for (n = 0; n < num_chars; n++) {
		/* set transformation */
		FT_Set_Transform(face, &matrix, &pen);

		/* load glyph image into the slot (erase previous one) */
#if USE_MONOCHROME
		error = FT_Load_Char(face, text[n], FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
#else
		error = FT_Load_Char(face, text[n], FT_LOAD_RENDER);
#endif
		if (error) {
			printf("error: %d\n", error);
			continue;	/* ignore errors */
		}

		/* now, draw to our target surface (convert position) */
		draw_bitmap(&slot->bitmap,
			    slot->bitmap_left,
			    target_height - slot->bitmap_top);

		dump_as_xbm(slot, text[n]);

		/* increment pen position */
		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
	}

	show_image();

	printf("global bbox: width: %ld, height: %ld (xMin: %ld, xMax: %ld, yMin: %ld, yMax: %ld)\n"
			, ((face->bbox.xMax >> 6) - (face->bbox.xMin >> 6))
			, ((face->bbox.yMax >> 6) - (face->bbox.yMin >> 6))
			, face->bbox.xMin >> 6
			, face->bbox.xMax >> 6
			, face->bbox.yMin >> 6
			, face->bbox.yMax >> 6
	      );
	printf("height: %d, ascent: %d, descent: %d\n", face->height >> 6, face->ascender >> 6, face->descender >> 6);

	FT_Done_Face(face);
	FT_Done_FreeType(library);
	return 0;
}
