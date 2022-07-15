cmake_minimum_required(VERSION 3.0)
project(fgui)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Create a compilation database symlink to configure clang tooling, ref.
# https://clang.llvm.org/docs/HowToSetupToolingForLLVM.html
file(CREATE_LINK
	${CMAKE_BINARY_DIR}/compile_commands.json
	${PROJECT_SOURCE_DIR}/compile_commands.json
	RESULT CREATE_SYMLINK_RESULT
	SYMBOLIC)
if(CREATE_SYMLINK_RESULT EQUAL 0)
	message(STATUS "Created ${PROJECT_SOURCE_DIR}/compile_commands.json for clang tooling")
else()
	# Do not abort on failure since the source directory might be read-only.
	message(WARNING "${CREATE_SYMLINK_RESULT}")
endif()
#set(CMAKE_C_CLANG_TIDY clang-tidy)
enable_testing()
add_subdirectory(src)
add_subdirectory(demo)
