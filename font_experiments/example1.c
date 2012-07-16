/* example1.c                                                      */
/*                                                                 */
/* This small program shows how to print a rotated string with the */
/* FreeType 2 library.                                             */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <ft2build.h>
#include FT_FREETYPE_H

//#define WIDTH   640
//#define HEIGHT  480
#define WIDTH  80
#define HEIGHT 16

/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];

/* Replace this function with something useful. */

void draw_bitmap(FT_Bitmap * bitmap, FT_Int x, FT_Int y)
{
	FT_Int i, j, p, q;
	FT_Int x_max = x + bitmap->width;
	FT_Int y_max = y + bitmap->rows;

	for (i = x, p = 0; i < x_max; i++, p++) {
		for (j = y, q = 0; j < y_max; j++, q++) {
			if (i < 0 || j < 0 || i >= WIDTH || j >= HEIGHT)
				continue;

			image[j][i] |= bitmap->buffer[q * bitmap->width + p];
		}
	}
}

void dump_as_xbm(FT_GlyphSlot glyphslot, char ch)
{
	FT_Int i, j, p, q;
	int x = glyphslot->bitmap_left;
	int y = glyphslot->bitmap_top;
	FT_Int x_max = x + glyphslot->bitmap.width;
	FT_Int y_max = y + glyphslot->bitmap.rows;

	printf("bitmap stuff: width: %d, rows: %d, pitch: %d, num_grays: %d\n"
			, glyphslot->bitmap.width
			, glyphslot->bitmap.rows
			, glyphslot->bitmap.pitch
			, glyphslot->bitmap.num_grays
			);

	printf("glyph->metrics stuff: width: %ld, height: %ld, horiBearingX: %ld, horiBearingY: %ld, horiAdvance: %ld\n"
			, glyphslot->metrics.width >> 6
			, glyphslot->metrics.height >> 6
			, glyphslot->metrics.horiBearingX >> 6
			, glyphslot->metrics.horiBearingY >> 6
			, glyphslot->metrics.horiAdvance >> 6
			);


	printf("#define %c_height %ld\n", ch, glyphslot->metrics.height >> 6);
	printf("#define %c_width %ld\n", ch, glyphslot->metrics.width >> 6);
	printf("static unsigned char %c_bits = {\n", ch);

	for (i = x, p = 0; i < x_max; i++, p++) {
		for (j = y, q = 0; j < y_max; j++, q++) {
			if (i < 0 || j < 0 || i >= WIDTH || j >= HEIGHT)
				continue;

			//image[j][i] |= bitmap->buffer[q * bitmap->width + p];
		}
	}
	printf("};\n");
	printf("\n");
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

	char *fontfilename;
	char *text;

	double angle;
	int target_height;
	int n, num_chars;

	if (argc != 3) {
		fprintf(stderr, "usage: %s font sample-text\n", argv[0]);
		exit(1);
	}

	fontfilename = argv[1];	/* first argument     */
	text = argv[2];		/* second argument    */
	num_chars = strlen(text);
	angle = (0 / 360.0) * 3.14159 * 2;	/* use N degrees     */
	target_height = HEIGHT;

	error = FT_Init_FreeType(&library);	/* initialize library */
	/* error handling omitted */

	error = FT_New_Face(library, fontfilename, 0, &face);	/* create face object */
	/* error handling omitted */

	/* use N pt at M dpi */
	//error = FT_Set_Char_Size(face, 0 * 64, 10 * 64, 72, 0);	/* set character size */
	/* error handling omitted */

	/* bfo: set character sizes directly in pixels, better for embedded */
	error = FT_Set_Pixel_Sizes(face, 0 /* pixel width */, 12 /* pixel height */);
	/* error handling omitted */

	slot = face->glyph;

	/* set up matrix */
	matrix.xx = (FT_Fixed) (cos(angle) * 0x10000L);
	matrix.xy = (FT_Fixed) (-sin(angle) * 0x10000L);
	matrix.yx = (FT_Fixed) (sin(angle) * 0x10000L);
	matrix.yy = (FT_Fixed) (cos(angle) * 0x10000L);

	/* the pen position in 26.6 cartesian space coordinates; */
	/* start at (300,200) relative to the upper left corner  */
	pen.x = 0 * 64;
	/* pen.y = ( target_height - HEIGHT + 4) * 64; */
	pen.y = (target_height - HEIGHT*0.7) * 64;

	for (n = 0; n < num_chars; n++) {
		/* set transformation */
		FT_Set_Transform(face, &matrix, &pen);

		/* load glyph image into the slot (erase previous one) */
		error = FT_Load_Char(face, text[n], FT_LOAD_RENDER); // | FT_LOAD_MONOCHROME ); // FT_LOAD_RENDER | FT_LOAD_MONOCHROME
		if (error)
			continue;	/* ignore errors */

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
