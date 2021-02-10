import os
import sys
import math

try:
    from PIL import Image, ImageDraw, ImageFont
except:
    print( "\nRequires the Pillow python module to produce font images\nPlease 'pip install pillow'" )
    exit(1)


def _make_pixel( val ):
    """
        Construct a pixel tuple of (R,G,B,A) from the unsigned integer val.
        Used to explicitly embed font metric data into the png
    """
    return ( (val & 0x000000ff, (val & 0x0000ff00) >> 8, (val & 0x00ff0000) >> 16, (val & 0xff000000) >> 24) )



def GenerateFont( fontName, pointSize, glyphset, fillColour=(255,255,255,255), outlineWidth=0, outlineColour=(255,255,255,255) ):
    """
        fontName –  A filename or file-like object containing a TrueType font. 
                    If the file is not found in this filename, the loader may also search in other directories, 
                    such as the fonts/ directory on Windows, /usr/share/fonts/ on Linux, or /Library/Fonts/, /System/Library/Fonts/ and ~/Library/Fonts/ on macOS.

        pointSize – The requested size, in points.

        glyphset  - A python set of unique characters ie.  set('a','b','c'... )

        fillColour - An (R,G,B,A) tuple for the glyph base colour

        outlineWidth - This will add an outline to the character (only supported partially currently as this messes with the font spacing which needs addressing properly)

        outlineColour - The colour used to render the character outline if its outlineWidth is not 0

        example :-  GenerateFont( "Arial.ttf", 30 )

                    Will attempt to create a font png with 30 point characters using Arial.ttf on the local machine.
                    The result will be a png file called "Arial.png" if successful. The resulting png has embedded 
                    font metric information on the last row of pixels and is used by the olcPGEX_Font class to configure the
                    font for rendering with both fixed size and proportional spacing.
    """
    assert( type(fontName) == str )
    assert( type(pointSize) == int and pointSize > 0 )
    assert( type(glyphset) == set )

    try:
        font = ImageFont.truetype( fontName, pointSize )
    except:
        print( f"\nUnable to locate the font file {fontName}\n" )
        return

    basefilename = os.path.basename( fontName )
    rootfilename = os.path.splitext(basefilename)[0]
    pngfilename = f'{rootfilename}.png'

    charwidth = 0
    charheight = 0
    
    # Build a list of character sizes for the loaded font and keep track of the max size in order to 
    # construct the output png image
    chars = list(glyphset)
    charSizes = []
    for c in chars:
        w,h = font.getsize( c )
        w += outlineWidth * 2
        h += outlineWidth * 2
        charSizes.append( (w,h) )
        charwidth = max(charwidth, w)
        charheight = max(charheight, h)

    # Add a 2 pixel buffer to counter any UV scaling issues
    charwidth += 2
    charheight += 2

    # All glyphs will be offset by 'xoffset' when being pasted onto the png image
    xoffset = 2

    # Construct the font png image - using an extra row which will contain the embedded font information
    charsPerRow = math.ceil( math.sqrt( len(chars) ) )
    
    nDataPixels = len(chars) * 2 + 5 # TODO: use this to ensure we have enough pixel rows to accommodate the data
    imageWidth = charsPerRow * charwidth
    nDataRows = math.ceil( nDataPixels / imageWidth )
    imageHeight = (charsPerRow * charheight) + nDataRows
    fontimage = Image.new("RGBA", (imageWidth, imageHeight), (0,0,0,0) )

    # Render each character into the png image
    dc = ImageDraw.Draw( fontimage )
    for i,c in enumerate(chars):
        dc.text( ((i % charsPerRow) * charwidth + xoffset + outlineWidth, (i // charsPerRow) * charheight + outlineWidth), c, font=font, fill=fillColour, stroke_width=outlineWidth, stroke_fill=outlineColour )

    # Write the font information into the last row of pixels
    # <"CFON"> <format_version> <xOffset> followed by 96 character widths
    datarow = fontimage.height - nDataRows
    format_version = 1
    fontimage.putpixel( (0, datarow), ( ord('C'), ord('F'), ord('O'), ord('N') )  )
    fontimage.putpixel( (1, datarow), _make_pixel(format_version) )
    fontimage.putpixel( (2, datarow), _make_pixel(len(chars)))
    fontimage.putpixel( (3, datarow), _make_pixel(xoffset) )
    fontimage.putpixel( (4, datarow), _make_pixel(charwidth) )
    fontimage.putpixel( (5, datarow), _make_pixel(charheight) )
    offset = 6
    for i,c in enumerate(charSizes):
        # Write the character width
        fontimage.putpixel( (offset % imageWidth, datarow + (offset // imageWidth)), _make_pixel(c[0]) )
        offset += 1
        # Write the unicode value
        fontimage.putpixel( (offset % imageWidth, datarow + (offset // imageWidth)), _make_pixel( ord(chars[i]) ) )
        offset += 1

    # Write the font png file with the font information embedded
    fontimage.save( pngfilename )
    print( f"Created font {pngfilename}" )



def _fail_with_usage():
    print( f"\nUsage: python generatecustomfont.py <fontName> <pointSize> <glyphset_filename>\n\n\tExample:  python generatecustomfont.py coolfont.ttf 30 glyphs.txt\n" )
    print( "<glyphset_filename> should be the name of a text file which contains every character you wish you include in your font\n" )
    exit(1)


if __name__ == "__main__":
    if len(sys.argv) < 4:
        _fail_with_usage()

    fontName, pointSize, glyphsetfilename = sys.argv[1:4]
    try:
        pointSize = int(pointSize)
    except:
        _fail_with_usage()

    if os.path.exists( glyphsetfilename ):
        with open( glyphsetfilename ) as f:
            text = f.read()
            glyphset = set()
            for c in text:
                glyphset.add( c )
    else:
        print( f"Unable to load the glyph set file {glyphsetfilename}\n" )

    GenerateFont( fontName, pointSize, glyphset )


