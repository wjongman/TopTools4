To compile this project, you need Borland C++Builder 5.0.
I never bothered upgrading to later versions because v5.0 worked fine. 
Later versions became too expensive for this pet-project and didn't seem to offer anything but more instability (the dreaded "Internal Compiler Error").
only offered more unneeded options instead of better stability
Although I didn't try myself, it should be possible to use the free-of-charge toolchain you can download from Embarcadero at:

http://forms.embarcadero.com/BCC32CompilerDownload

Create obj and exe directories.

First you need to build gif.lib, png.lib and TopToolParts.bpl. 

TopToolParts.bpl also needs to be installed in the Components Palette, it contains some custom components I made when designing the Options dialog.

Resource compiler