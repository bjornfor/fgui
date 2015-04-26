# YouCompleteMe Vim plugin config

import os
import ycm_core
import subprocess

# These are the compilation flags that will be used in case there's no
# compilation database set (by default, one is not set).
flags = [
    '-Wall',
    '-Wextra',
    '-Werror',
    '-fexceptions',
    #'-DNDEBUG',
    # THIS IS IMPORTANT! Without a "-std=<something>" flag, clang won't know which
    # language to use when compiling headers. So it will guess. Badly. So C++
    # headers will be compiled as C headers. You don't want that so ALWAYS specify
    # a "-std=<something>".
    # For a C project, you would set this to something like 'c99' instead of
    # 'c++11'.
    '-std=c99',
    # ...and the same thing goes for the magic -x option which specifies the
    # language that the files to be compiled are written in. This is mostly
    # relevant for c++ headers.
    # For a C project, you would set this to 'c' instead of 'c++'.
    '-x',
    'c',
]

def is_c_source_file(filename):
    return filename[-2:] == ".c"

def is_c_header_file(filename):
    return filename[-2:] == ".h"

def is_cxx_file(filename):
    return filename[-2:] in (".cpp", ".cxx")

def is_cxx_header(filename):
    return filename[-2:] in (".hpp", ".hxx")

def get_proj_flags():
    output = subprocess.check_output("pkg-config --cflags --libs sdl", shell=True)
    return output.split()

flags.extend(get_proj_flags())

# youcompleteme is calling this function to get flags
# You can also set database for flags. Check: JSONCompilationDatabase.html in
# clang-3.2-doc package
def FlagsForFile(filename):
    return {
        'flags': flags,
        'do_cache': True
    }
