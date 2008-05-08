PixelSnif(fer)
Pixel
Pixel
Pixel
Pixel
PixelMate


TopTools preliminary Help File

TopTools is a set of utilities I developed through the years as I was confronted with questions like:

How many pixels should I move this button to align with the rest?
How can I quickly resize this window to 640x480
What is the height of this image?
What is the RGB value of this pixel?
How can I get a closer look at this icon?
How can I get this piece of the screen on the clipboard?
What was the ASCII value of this character again?
What is 10110110 in hex?

Description:

Set of utilities to examine and measure screen elements at pixel level, helpful for web-designers and GUI-developers.
Features a loupe and a ruler, seamlessly working together.
Additional are a base-converter, a color-picker and a screen-grabber.
The main window has a small footprint to save desktop real estate.


History:

TopTools was born when I needed a means to examine and measure screen elements on a pixel-by-pixel basis.
At that time I was using both a screen-magnifier and a screen-ruler, but the two didn't really cooperate:
The hotspot of the magnifier was in the bottom-right corner of the viev, instead of in the center, so when the magnifier was used together with the ruler, one would see either a close-up of the ruler, or a close-up of what was really to be measured, but with the ruler out of sight.
The ruler did accept up/down/left/right keyboard-commands to move itself respectively, but it didn't allow me to move the mousecursor by similar means.
The ruler was actually quite good, but it came up with annoying shareware nag-boxes and I thought the price was a bit over the edge for such a simple util, so I decided to create my own..

I wrote the initial version using Borland C++ 5.0 and OWL at about the same time C++Builder3 arrived. I fell in love with BCB3 and porting TopTools was one of my first projects using this great development environment.


Features:

General:
All tool windows have a right-click popup menu, most of them are self-explanatory.

Loupe:
The loupe displays an enlarged view of the area under the mousepointer, with optional a crosshair and/or a grid.
The zoom factor can be adjusted between 5 8 10 16 and 20 times.
The loupeview can be frozen by pressing the spacebar (the loupe needs to have keyboard focus for this to work, you can give it focus by clicking on it's surface, focus is indicated by the color of the border: red means focussed, grey means not)

Ruler:
The ruler measures screen objects in pixel units. It has a floating position indicator that follows the mousepointer. The mousepointer has a line shape that flips when moved across the center of the ruler, in this way both sides of the ruler can be used.


Info:

Sceen grabber: