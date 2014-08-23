set(SOURCES
	fgui_primitives.c
	fgui_font.c
	fgui_button.c
	fgui_label.c
	fgui_combobox.c
	fgui_lineedit.c
	fgui_widget.c
	fgui_application.c
	fonts/courier.c
	fgui_3d.c
	)
set(HEADERS
	fgui_application.h
	fgui_button.h
	fgui_combobox.h
	fgui_event.h
	fgui_font_data.h
	fgui_font.h
	fgui.h
	fgui_label.h
	fgui_lineedit.h
	fgui_primitives.h
	fgui_types.h
	fgui_widget.h
	fgui_3d.h
	utils.h
	)
add_library(fgui STATIC ${SOURCES})
install(TARGETS fgui DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/fgui)
