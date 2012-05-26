fgui
====
Bjørn Forsman

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

The 'f' in fgui is either the first letter of the author's last name or the
letter that comes before the 'g' in gui (because fgui is very lightweight).


Implemented features
--------------------

None so far.


Planned features (more or less in sequential order)
---------------------------------------------------

* testing framework (use SDL?)
* basic drawing primitives: rectangle, line, circle
* monospaced font drawing (how to generate fonts?)
* widgets:
** button
** label
** textedit (single line and multi-line)
** combobox
** hierarchical menu (submenus and actions)
* find a way to dispatch input events to the right widget
* input validators (for textedit)
