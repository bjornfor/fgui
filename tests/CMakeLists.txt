find_package(SDL REQUIRED)
include_directories(${SDL_INCLUDE_DIR})
add_executable(fgui_test fgui_test.c)
target_link_libraries(fgui_test fgui ${SDL_LIBRARY})
