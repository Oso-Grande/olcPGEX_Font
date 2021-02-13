# olcPGEX_Font and olcPGEX_CustomFont
A couple of extensions to the olcPixelGameEngine for exporting and rendering your own fonts at any point size and orientation


## What is it?

This code allows you to generate and render truetype fonts (with limitations explained below) using the one lone coder pixel game engine (olcPGE).

The text rendering is kept deliberately simple and in line with the original olc DrawString...Decal() functions and in fact is simple the original code modified to use
a different sprite/decal and font metrics.

The extensions are comprised of 2 extension classes derived from olc::PGEX in the olc namespace called **olc::Font** and **olc::CustomFont**

the **olc::Font** class is used to render fonts which contain characters in the ASCII range 32-127.

the **olc::CustomFont** is used when you want something that will render a subset of any unicode characters.

To use the classes you must include the header file olcPGEX_Font.h/CustomFont.h after defining either OLC_PGEX_FONT or OLC_PGEX_CUSTOMFONT

```
#define OLC_PGEX_FONT
#include "olcPGEX_Font.h"
```

```
#define OLC_PGEX_CUSTOMFONT
#include "olcPGEX_CustomFont.h"
```

#####See the Example.cpp and CustomFontExample.cpp files for usage.

If you have used DrawString...() in olcPixelGameEngine you will already be familiar with the functions, with a couple of additions for drawing rotated text.


## Creating an ASCII font (for use with olc::Font)

A ascii font is simply a png file that has been generated using the supplied script (generatefont.py) 
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



## Creating a custom unicode font (for use with olc::CustomFont)

A custom font is simply a png file that has been generated using the supplied script (generatecustomfont.py) 
The script generatecustomfont.py takes the following arguments on the command line : a font file name, a pointsize and a text filename (the text file should be utf-8 encoded and contain all the unicode characters you want in your resulting font file).

eg. 
```
python generatecustomfont.py keifont.ttf 40 glyphs_required.txt
```

(You will need a python3.6 and above interpreter with the Pillow module installed. [pip install pillow] )

####Important:
The file ***glyphs_required.txt*** in the example above is a txt file that can contain any unicode characters you wish to be able to render. This text file should be utf-8 encoded. A list of used unique characters will be extracted and those characters will be built into the png along with the unicode mappings and glyph positions (as data embedded into the png pixel data itself). The olc::CustomFont() constructor will detect that this png is a custom font and be able to render the glyphs within it.

Example of the contents of glyphs_required.txt :-
```
こんにちは世界
```

####NOTE:Only the characters in this text file will be part of the font.


This special png actually contains not only the glyphs but also the necessary font metrics encoded into the pixel data itself.
The olc::CustomFont() constructor will attempt to discover the embedded metrics in the png and use them for rendering.
(If the png doesn't look like an exported custom font then no characters will be rendered if you try to use the font for Drawing)


## Limitations

There are several limitations currently, including but not limited to:-

* With olc::Font only the characters in the range ascii 32 - 127 are supported as per the inbuilt olc PGE font.
* With olc::CustomFont only the characters you specify in a text file when you export the font png file (using gereratecustomfont.py) will be supported.
* Only fonts which can be found and loaded by the Pillow ImageFont truetype loader are supported (luckily this means most fonts you will come across in ttf format).
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
