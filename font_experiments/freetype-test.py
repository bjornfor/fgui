#!/usr/bin/env python

import freetype

# face = freetype.Face('Vera.ttf')
face = freetype.Face('/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-B.ttf')
face.set_char_size(9*64)
face.load_char('i')
bitmap = face.glyph.bitmap
print bitmap.buffer
