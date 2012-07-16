CC = $(CROSS_COMPILE)gcc
CFLAGS = $(shell freetype-config --cflags) -g -Wall
LIBS = $(shell freetype-config --libs) -lm

PROG = example1 font2xbm

all: $(PROG)

#have_freetype_config:
#@freetype-config2 --cflags --libs >/dev/null  # see if we can run freetype-config

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(PROG)

.PHONY: all clean have_freetype_config
