/* generated file from command-line "./genfont.py abcæ output/" */

#include <stdint.h>
#include <stdlib.h>  /* defines wchar_t for platforms that do not have wchar.h? */

/* available unicode codepoints */
uint8_t cAscii[4] = {97,99,98,230,};

/* width and height of the characters in bits */
uint8_t cWidth[4] = {10,10,10,10,};
uint8_t cHeight[4] = {15,15,15,15,};

uint16_t cData[4] = {15,15,15,15,};


int i;

for (i = 0; i < ARRAY_LEN(cUnicode); i++) {
	if (cUnicode[i] == 'a') {
		break;
	}
}


struct font_data {
	wchar_t codepoint;
	int width;
	int height;
	unsigned char *xbmdata;
};
