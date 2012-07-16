http://stackoverflow.com/questions/2156572/c-header-file-with-bitmapped-fonts

# generate the bitmap of 'A'
$ convert -resize 9x13\! -font Nimbus-Mono-Regular -pointsize 10 label:A A.xbm

# view it
$ xdg-open A.xbm

Actually, the default font (drop -font FONTNAME) looks better.

List fonts known to imagemagick (either internal fonts or system fonts):
$ convert -list font
