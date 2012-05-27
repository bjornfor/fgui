set(SOURCES
	fgui_primitives.c
	fgui_font.c
	fgui_button.c
	fonts/courier.c
	)
add_library(fgui STATIC ${SOURCES})
