# olcPGEX_Font
An extension to the olcPixelGameEngine for exporting and rendering your own fonts at any point size and orientation


## What is it?

This code allows you to generate and render truetype fonts (with limitations explained below) using the one lone coder pixel game engine (olcPGE).

The text rendering is kept deliberately simple and in line with the original olc DrawString...Decal() functions and in fact is simple the original code modified to use
a different sprite/decal and font metrics.

The extension is comprised of an extension class derived from olc::PGEX in the olc namespace called olc::Font
To use the class you must include the header file olcPGEX_Font.h after defining OLC_PGEX_FONT

```
#define OLC_PGEX_FONT
#include "olcPGEX_Font.h"
```

See the Example.cpp file for usage. If you have used DrawString...() in olcPixelGameEngine you will already be familiar with the functions, with a couple of additions for drawing rotated text.


## Creating a font

A font is simply a png file that has been generated using the supplied script (generatefont.py) 
The script generatefont.py takes a font file and a pointsize as command line arguments and generates a png.

eg. 
```
python generatefont.py Arial.ttf 40
```

This will generate a file called Arial.png which has the glyphs from the Arial font at a size of 40 points.
(You will need a python3.6 and above interpreter with the Pillow module installed. [pip install pillow] )

This special png actually contains not only the glyphs but also the necessary font metrics encoded into the pixel data itself.
The olc::Font() constructor will attempt to discover the embedded metrics in the png and use them for rendering.
(If the png doesn't look like an exported font with embedded metrics then it will default to simply assuming the png contains 6 rows of 16 characters of fixed size)

The included examples are fonts which I sourced from a commerical free website - all the licenses and restrictions are included. Please don't use them unless you observe those restrictions/licences.

To generate the fonts I executed these commands (the fonts can be found at 1001fonts.com) :-

```
python generatefont.py the_unseen.ttf 40
python generatefont.py underworld.ttf 40
python generatefont.py Stars.ttf 60
python generatefont.py armalite.ttf 50
```



## Limitations

There are several limitations currently, including but not limited to:-

* Only the characters in the range ascii 32 - 127 are supported as per the inbuilt olc PGE font.
* Only fonts which can be found and loaded by the Pillow ImageFont truetype loader are supported (luckily this means most fonts you will come across in ttf format)
* Cursive or script type fonts (those which have overlapping characters when rendered proportionally) are not currently supported since only a simple character width metric is used. Try it and see. Feel free to extend the metrics yourself and make it work :)
* Adding font outlines is possible if you examine the python script, but the tool doesn't use it yet since it doesn't account for the spacing differences yet. This may or may not be added later.



## License

Copyright 2021 OsoGrande with limitations set by the below OLC-3 license since this is a derived work.
Free to use/modify in any context, as long as you observe the conditions of the third party license below.


## Third Party License
## One Lone Coder (Author of the olcPixelGameEngine)

## License (OLC-3)

Copyright 2018, 2019, 2020 OneLoneCoder.com

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions 
are met:

1. Redistributions or derivations of source code must retain the above 
   copyright notice, this list of conditions and the following disclaimer.

2. Redistributions or derivative works in binary form must reproduce 
   the above copyright notice. This list of conditions and the following 
   disclaimer must be reproduced in the documentation and/or other 
   materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its 
   contributors may be used to endorse or promote products derived 
   from this software without specific prior written permission.
    
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
