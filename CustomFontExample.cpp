#include <memory>
#include <cmath>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_CUSTOMFONT
#include "olcPGEX_CustomFont.h"


class Example : public olc::PixelGameEngine
{
private:
    std::unique_ptr<olc::CustomFont> keiFont;
    std::unique_ptr<olc::CustomFont> emojiFont;

public:
	Example()
	{
		sAppName = "Font Extension Example";
	}

public:
	bool OnUserCreate() override
	{
        // Create the font objects now the engine is started
        // The font pngs have been created using the python script generatecustomfont.py
        // eg.    python generatecustomfont.py keifont.ttf 50 glyphs.txt
        // where glyphs.txt is a txt file containing the unicode characters you wish to incorporate into your bitmap font
        // Only the unique characters in the txt file will be built into the font png file
        // Any characters referenced in the wstrings in the call to DrawString...() will be ignored if they are not included in the font
        keiFont = std::make_unique<olc::CustomFont>( "./keifont.png" );
        emojiFont = std::make_unique<olc::CustomFont>( "./NotoEmoji-Regular.png" );

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        Clear( olc::WHITE );

        static float fAngle = 0.0f;
        fAngle += fElapsedTime;

        // Create some text to render - notice using wstring 
        std::wstring kei_text   = L"こんにちは世界";
        std::wstring emoji_text = L"😊😉😍😘😚😜😂\n😝😳😁😣😢😭😰";

        // Get the string dimensions for rendering proportionally
        auto kei_size   = keiFont->GetTextSizeProp( kei_text );
        auto emoji_size = emojiFont->GetTextSizeProp( emoji_text );

        // Compute the centre points so we can rotate about them
        auto kei_centre = kei_size / 2.0f;
        auto emoji_centre = emoji_size / 2.0f;

        auto fScale = 0.5f + std::abs( std::cos( fAngle ) );

        // Example of rendering proportionally, with a scale and a rotation
        keiFont->DrawRotatedStringPropDecal( {400, 200}, kei_text, fAngle, kei_centre, olc::Pixel(255,200,0,255), {fScale, fScale}  );
        emojiFont->DrawRotatedStringPropDecal( {400,400}, emoji_text, fAngle, emoji_centre, olc::BLACK, {fScale, fScale}  );
        
        return true;
	}
};


int main()
{
	Example demo;
	if (demo.Construct(800, 600, 1, 1))
		demo.Start();

	return 0;
}

