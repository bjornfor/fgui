#!/usr/bin/env python
"""
Generate .xbm font files for a given character set and font type.
"""

import os
import sys
import sets
import errno
import subprocess
from optparse import OptionParser


__version__ = "0.x"

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as e:
        if e.errno == errno.EEXIST:
            pass
        else:
            raise

def parse_args():
    usage = "Usage: %prog [options] <list-of-characters> <output-directory>\n" \
            "\n" \
            "Generate an .xbm file in the output directory for each\n" \
            "character in the character list (duplicates are removed).\n" \
            "This tool depends on the 'gm' binary from GraphicsMagick."
    parser = OptionParser(usage=usage, version=__version__)
    parser.add_option("-f", "--font", dest="font", default="Courier",
                      help="render text with this font, default \"%default\" \
                      (see 'convert -list font' for available fonts)")
    parser.add_option("-s", "--size", dest="size", default="10x14",
                      help="character size in pixels, default \"%default\"")
    parser.add_option("-d", "--density", dest="density", type="int", default=500,
                      help="density (passed to graphicsmagick), default \"%default\"")
    parser.add_option("-p", "--pointsize", dest="pointsize", type="int", default=14,
                      help="font point size (passed to graphicsmagick), default \"%default\"")
    (options, args) = parser.parse_args()
    return (options, args)


def main():
    options, args = parse_args()
    if len(args) != 2:
        print("ERR: missing argument(s). Try '--help'.")
        sys.exit(1)
    # convert to set to remove duplicate characters
    characters = sets.Set(unicode(args[0], 'utf8'))
    # convert back to list so we can sort
    characters = list(characters)
    characters.sort()
    outputdir = args[1]

    try:
        #width, height = map(int, options.size.split("x"))
        pass
    except ValueError as e:
        print("ERROR: %s" % e)
        sys.exit(1)

    print("characters: \"%s\", outputdir: %s" % (characters, outputdir))
    mkdir_p(outputdir)

    for ch in characters:
        path = os.path.join(outputdir, "codepoint_%s.xbm" % ord(ch))
        cmdlist = ["gm", "convert"]
        if options.size:
            cmdlist.extend(["-resize", options.size])
        #cmdlist.extend(["-pointsize", "%s" % options.pointsize])
        cmdlist.extend(["-density", "%d" % options.density])
        cmdlist.extend(["-font", options.font])
        cmdlist.extend(["label:%s" % ch, path])
        print(" ".join(cmdlist))
        subprocess.check_call(cmdlist)

    # build an array that connects character code points to bitmaps
    num_chars = len(characters)
    max_bitmap_size = 0

    # write code file
    path = os.path.join(outputdir, "font.c")
    generated_notice = "/* generated file from command-line \"%s\" */\n" % (" ".join(sys.argv))
    includes = "\n#include <stdint.h>\n#include \"font.h\"\n"
    with open(path, "w") as f:
        f.write(generated_notice + includes)

        #f.write("\n/* available unicode codepoints */\n")
        #f.write("uint8_t cAscii[%d] = {" % num_chars)
        #for ch in characters:
        #    f.write("%d," % ord(ch))
        #f.write("};\n")

        #f.write("\n/* width and height of the characters in bits */\n")
        #f.write("uint8_t cWidth[%d] = {" % num_chars)
        #for ch in characters:
        #    f.write("%d," % width)
        #f.write("};\n")
        #f.write("uint8_t cHeight[%d] = {" % num_chars)
        #for ch in characters:
        #    f.write("%d," % height)
        #f.write("};\n")

        f.write("\nstruct font_data cUnicode[%d] = {\n" % num_chars)
        for i, ch in enumerate(characters):
            xbmbits = ""
            with open(os.path.join(outputdir, "codepoint_%s.xbm" % ord(ch))) as xbmfile:
                data_section = False
                width, height = None, None
                for line in xbmfile:
                    if not width:
                        width = int(line.split()[-1])
                        continue
                    if not height:
                        height = int(line.split()[-1])
                        continue
                    if data_section:
                        xbmbits += line.strip() + " "
                    if line.endswith("{\n"):
                        data_section = True
            idx = xbmbits.index("}")
            xbmbits = xbmbits[:idx]
            xbmbits = xbmbits.strip()
            len_xbmbits = len(xbmbits.split())
            if len_xbmbits > max_bitmap_size:
                max_bitmap_size = len_xbmbits
            f.write("{.codepoint = %d, .width = %d, .height = %d, .data = {%s}},\n\n" % (ord(ch), width, height, xbmbits))
        f.write("};\n")

    # write header file
    path = os.path.join(outputdir, "font.h")
    guard_macro_start = "\n#ifndef FONT_H\n#define FONT_H\n"
    includes = "\n#include <stdint.h>\n#include <stddef.h>  /* for wchar_t */\n"
    with open(path, "w") as f:
        f.write(generated_notice + guard_macro_start + includes)
        f.write("""\nstruct font_data {
    wchar_t codepoint;
    int width;
    int height;
    unsigned char data[%d];
};

extern struct font_data cUnicode[%d];
""" % (max_bitmap_size, num_chars))
        f.write("\n#endif /* FONT_H */")

if __name__ == '__main__':
    main()
