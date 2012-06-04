fgui
====
Bjørn Forsman <bjorn (dot) forsman (at) gmail (dot) com>

fgui is a simple GUI library, intended for small embedded systems (e.g. 8-bit
MCUs). fgui has basic drawing primitives (line, rectangle, circle) and some GUI
widgets. All widgets look terribly ugly.

fgui has no event loop. To make fgui usable in different contexts, the user has
to manage the event loop and call fgui to do event processing. The user also
has to setup the drawing surface/screen and whenever fgui needs to draw
something it will call

  fgui_set_pixel(uint16_t x, uint16_t y, uint32_t color)

which must be supplied by the user. The color value is interpreted like this:
the most significant byte is ignored and the three next less significant bytes
are the R, G and B color values. For monochrome operation use color=0 or
color=1.

fgui does not use dynamic memory allocation.

The 'f' in fgui is either the first letter of the author's last name or the
letter that comes before the 'g' in gui (because fgui is very lightweight).


Implemented features
--------------------

* basic drawing primitives:
** line
** rectangle (empty + filled)
** circle (empty + filled)
** triangle (only empty)
* monospaced font
* widgets:
** label
** push button - supports "on click" callback
** lineedit - single line text input
** combobox - supports "item changed" callback
* example/test program using fgui with SDL, http://www.libsdl.org/


Possible features
-----------------

* widgets:
** hierarchical menu (submenus and actions)
* widget coordinates relative to parent
* make parent widgets draw their children (currently all widgets must be drawn
  'manually' by the application)
* clipping (widgets should not draw anything outside their border)
* input validators (for textedit)


License
-------

fgui is released under the MIT open source license. See LICENSE.txt or go to
http://www.opensource.org/licenses/MIT for full license text.
