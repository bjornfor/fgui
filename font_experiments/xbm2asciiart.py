#!/usr/bin/env python
"""
Take an .xbm file and print an ascii representation of it to stdout.
"""

import sys
import fileinput
from optparse import OptionParser

__version__ = "0.x"

def parse_args():
    usage = "Usage: %prog [options] [xbm_file]\n" \
            "\n" \
            "Print an ascii art representation of an .xbm file to stdout.\n" \
            "If no file is given, read from stdin."
    parser = OptionParser(usage=usage, version=__version__)
    #parser.add_option("-p", "--pointsize", dest="pointsize", type="int", default=50,
                      #help="font point size passed to convert, default \"%default\"")
    (options, args) = parser.parse_args()
    return (options, args)

def main():
    options, args = parse_args()
    if len(args) == 0:
        xbmfile = sys.stdin
    else:
        xbmfile = open(args[0])

    #for line in fileinput.input():
        #print("line: %s" % line)

    for line in xbmfile:
        print("line: %s" % line.strip())

    xbmfile.close()

if __name__ == '__main__':
    main()
