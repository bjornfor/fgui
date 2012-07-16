/* generated file from command-line "./genfont.py Aa output" */

#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include <stddef.h>  /* for wchar_t */

struct font_data {
    wchar_t codepoint;
    int width;
    int height;
    unsigned char data[24];
};

extern struct font_data cUnicode[2];

#endif /* FONT_H */