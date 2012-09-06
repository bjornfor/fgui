/*
 * Copyright (c) 2012 Bjørn Forsman
 *
 * This file is part of fgui which is released under the MIT license. See
 * LICENSE.txt or go to http://www.opensource.org/licenses/MIT for full
 * license text.
 */

/**
 * @file
 *
 * fgui event types.
 */

#ifndef FGUI_EVENT_H
#define FGUI_EVENT_H

/*
 * Unicode C0 Controls and Basic Latin codes (equal to ASCII), with some holes
 * in it :-)  Also, there is a mapping of arrow keys after codepoint 127.
 */
enum fgui_key {
	FGUI_KEY_NULL                 = 0x0000,

	FGUI_KEY_BACKSPACE            = 0x0008,
	FGUI_KEY_TAB                  = 0x0009,

	FGUI_KEY_RETURN               = 0x000d,

	FGUI_KEY_ESCAPE               = 0x001b,

	FGUI_KEY_SPACE                = 0x0020,
	FGUI_KEY_EXCLAMATION_MARK     = 0x0021,
	FGUI_KEY_QUOTATION_MARK       = 0x0022,
	FGUI_KEY_NUMBER_SIGN          = 0x0023,
	FGUI_KEY_DOLLAR_SIGN          = 0x0024,
	FGUI_KEY_PERCENT_SIGN         = 0x0025,
	FGUI_KEY_AMPERSAND            = 0x0026,
	FGUI_KEY_APOSTROPHE           = 0x0027,
	FGUI_KEY_LEFT_PARENTHESIS     = 0x0028,
	FGUI_KEY_RIGHT_PARENTHESIS    = 0x0029,
	FGUI_KEY_ASTERISK             = 0x002a,
	FGUI_KEY_PLUS                 = 0x002b,
	FGUI_KEY_COMMA                = 0x002c,
	FGUI_KEY_MINUS                = 0x002d,
	FGUI_KEY_FULL_STOP            = 0x002e,
	FGUI_KEY_SLASH                = 0x002f,
	FGUI_KEY_0                    = 0x0030,
	FGUI_KEY_1                    = 0x0031,
	FGUI_KEY_2                    = 0x0032,
	FGUI_KEY_3                    = 0x0033,
	FGUI_KEY_4                    = 0x0034,
	FGUI_KEY_5                    = 0x0035,
	FGUI_KEY_6                    = 0x0036,
	FGUI_KEY_7                    = 0x0037,
	FGUI_KEY_8                    = 0x0038,
	FGUI_KEY_9                    = 0x0039,
	FGUI_KEY_COLON                = 0x003a,
	FGUI_KEY_SEMICOLON            = 0x003b,
	FGUI_KEY_LESS_THAN            = 0x003c,
	FGUI_KEY_EQUALS               = 0x003d,
	FGUI_KEY_GREATER_THAN         = 0x003e,
	FGUI_KEY_QUESTION_MARK        = 0x003f,
	FGUI_KEY_AT_SIGN              = 0x0040,
	FGUI_KEY_A                    = 0x0041,
	FGUI_KEY_B                    = 0x0042,
	FGUI_KEY_C                    = 0x0043,
	FGUI_KEY_D                    = 0x0044,
	FGUI_KEY_E                    = 0x0045,
	FGUI_KEY_F                    = 0x0046,
	FGUI_KEY_G                    = 0x0047,
	FGUI_KEY_H                    = 0x0048,
	FGUI_KEY_I                    = 0x0049,
	FGUI_KEY_J                    = 0x004a,
	FGUI_KEY_K                    = 0x004b,
	FGUI_KEY_L                    = 0x004c,
	FGUI_KEY_M                    = 0x004d,
	FGUI_KEY_N                    = 0x004e,
	FGUI_KEY_O                    = 0x004f,
	FGUI_KEY_P                    = 0x0050,
	FGUI_KEY_Q                    = 0x0051,
	FGUI_KEY_R                    = 0x0052,
	FGUI_KEY_S                    = 0x0053,
	FGUI_KEY_T                    = 0x0054,
	FGUI_KEY_U                    = 0x0055,
	FGUI_KEY_V                    = 0x0056,
	FGUI_KEY_W                    = 0x0057,
	FGUI_KEY_X                    = 0x0058,
	FGUI_KEY_Y                    = 0x0059,
	FGUI_KEY_Z                    = 0x005a,
	FGUI_KEY_LEFT_SQUARE_BRACKET  = 0x005b,
	FGUI_KEY_BACKSLASH            = 0x005c,
	FGUI_KEY_RIGHT_SQUARE_BRACKET = 0x005d,
	FGUI_KEY_CIRCUMFLEX_ACCENT    = 0x005e,
	FGUI_KEY_UNDERSCORE           = 0x005f,
	FGUI_KEY_GRAVE_ACCENT         = 0x0060,
	FGUI_KEY_a                    = 0x0061,
	FGUI_KEY_b                    = 0x0062,
	FGUI_KEY_c                    = 0x0063,
	FGUI_KEY_d                    = 0x0064,
	FGUI_KEY_e                    = 0x0065,
	FGUI_KEY_f                    = 0x0066,
	FGUI_KEY_g                    = 0x0067,
	FGUI_KEY_h                    = 0x0068,
	FGUI_KEY_i                    = 0x0069,
	FGUI_KEY_j                    = 0x006a,
	FGUI_KEY_k                    = 0x006b,
	FGUI_KEY_l                    = 0x006c,
	FGUI_KEY_m                    = 0x006d,
	FGUI_KEY_n                    = 0x006e,
	FGUI_KEY_o                    = 0x006f,
	FGUI_KEY_p                    = 0x0070,
	FGUI_KEY_q                    = 0x0071,
	FGUI_KEY_r                    = 0x0072,
	FGUI_KEY_s                    = 0x0073,
	FGUI_KEY_t                    = 0x0074,
	FGUI_KEY_u                    = 0x0075,
	FGUI_KEY_v                    = 0x0076,
	FGUI_KEY_w                    = 0x0077,
	FGUI_KEY_x                    = 0x0078,
	FGUI_KEY_y                    = 0x0079,
	FGUI_KEY_z                    = 0x007a,
	FGUI_KEY_LEFT_CURLY_BRACKET   = 0x007b,
	FGUI_KEY_VERTICAL_BAR         = 0x007c,
	FGUI_KEY_RIGHT_CURLY_BRACKET  = 0x007d,
	FGUI_KEY_TILDE                = 0x007e,
	FGUI_KEY_DELETE               = 0x007f,

	/*
	 * Custom non-printable keys should ideally not clash with other
	 * existing codepoints, but there seems to be no better way right now.
	 * Private Use Area (PUA) in Unicode (0xe000 - 0xf8ff) seemed
	 * promising, but it seems they are reserved for *printable* charcodes.
	 * According to the Unicode standard(?), it is basically up to the
	 * application do define the meaning of the C1 Control codes U+0080 -
	 * U+009f. If the application does not specify semantics, one can fall
	 * back to the semantics specified in ISO/IEC 6429.
	 *
	 * Also, isprint(x) where x >= 256 may wrap around. Until fgui uses
	 * wide chars we keep the key codes within 0-255.
	 *
	 * TODO: solve this issue by completely separating keys from text?
	 */
	FGUI_KEY_ARROW_UP             = 0x0080,
	FGUI_KEY_ARROW_DOWN           = 0x0081,
	FGUI_KEY_ARROW_LEFT           = 0x0082,
	FGUI_KEY_ARROW_RIGHT          = 0x0083,
};

enum fgui_event_type {
	FGUI_EVENT_KEYDOWN,
	FGUI_EVENT_KEYUP,
};

struct fgui_keyboard_event {
	unsigned int keycode;
};

struct fgui_event {
	enum fgui_event_type type;
	union {
		struct fgui_keyboard_event key;
		// extend with other event types here
	};
};

#endif /* FGUI_EVENT_H */
