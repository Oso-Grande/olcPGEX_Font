#pragma once

#ifndef __OLC_PGEX_CUSTOMFONT__
#define __OLC_PGEX_CUSTOMFONT__

#include <vector>
#include <memory>
#include "olcPixelGameEngine.h"

namespace olc
{
	class CustomFont : public olc::PGEX
	{
	public:
        CustomFont() = delete;
	    CustomFont(const std::string& sFontFile, olc::ResourcePack* pack = nullptr);
		
		olc::vi2d GetTextSize(const std::wstring& s);
		olc::vi2d GetTextSizeProp(const std::wstring& s);
        
        void DrawStringDecal(const olc::vf2d& pos, const std::wstring& sText, const Pixel col = olc::WHITE, const olc::vf2d& scale = { 1.0f, 1.0f });
        void DrawRotatedStringDecal(const olc::vf2d& pos, const std::wstring& sText, const float fAngle, const olc::vf2d& center = {0.0f, 0.0f},  
                                    const Pixel col = olc::WHITE, const olc::vf2d& scale = { 1.0f, 1.0f });

		void DrawStringPropDecal(const olc::vf2d& pos, const std::wstring& sText, const Pixel col = olc::WHITE, const olc::vf2d& scale = { 1.0f, 1.0f });
		void DrawRotatedStringPropDecal(const olc::vf2d& pos, const std::wstring& sText, const float fAngle, const olc::vf2d& center = {0.0f, 0.0f},  
                                        const Pixel col = olc::WHITE, const olc::vf2d& scale = { 1.0f, 1.0f });

    private:
        std::unique_ptr<olc::Sprite>    fontSprite;
        std::unique_ptr<olc::Decal>     fontDecal;

        float                           fCharWidth;
        float                           fCharHeight;
        std::unordered_map<uint32_t, std::pair<olc::vi2d, olc::vi2d> > mGlyphPositionsMono;
        std::unordered_map<uint32_t, std::pair<olc::vi2d, olc::vi2d> > mGlyphPositionsProp;
    };
}


#ifdef OLC_PGEX_CUSTOMFONT
#undef OLC_PGEX_CUSTOMFONT

namespace olc
{
	CustomFont::CustomFont(const std::string& sFontFile, olc::ResourcePack* pack)
	{
        fontSprite = std::make_unique<olc::Sprite>( sFontFile, pack );
        fontDecal = std::make_unique<olc::Decal>( fontSprite.get() );

        // Find the CFNT signature and extract the embedded font information
        int dataRow = -1;
        for( auto i = fontSprite->height-1; dataRow<0 && i>-1; i-- )
        {
            if( fontSprite->GetPixel( 0, i ) == olc::Pixel('C','F','O','N') )
                dataRow = i;
        }

        if( dataRow > -1 )
        {
            // CFNT signature found - presume the pixels contain valid font information
            uint32_t nFormatVersion = fontSprite->GetPixel( 1, dataRow ).n;
            uint32_t nGlyphs        = fontSprite->GetPixel( 2, dataRow ).n;
            int nOffset             = fontSprite->GetPixel( 3, dataRow ).n;
            int nCharWidth          = fontSprite->GetPixel( 4, dataRow ).n;
            int nCharHeight         = fontSprite->GetPixel( 5, dataRow ).n;
            uint32_t nCharsPerRow   = fontSprite->width / nCharWidth;

            for( auto i=0; i < nGlyphs; i++ )
            {
                int pixelIndex      = i*2 + 6;
                int pixelX          = pixelIndex % fontSprite->width;
                int pixelY          = pixelIndex / fontSprite->width + dataRow;
                int nWidth          = fontSprite->GetPixel( pixelX, pixelY ).n;
                uint32_t nGlyph     = fontSprite->GetPixel( pixelX+1, pixelY ).n;
                int x               = i % nCharsPerRow * nCharWidth + nOffset;
                int y               = i / nCharsPerRow * nCharWidth;
                mGlyphPositionsMono.emplace( nGlyph, std::make_pair<olc::vi2d, olc::vi2d>( {x,y}, {nCharWidth,nCharHeight-1}) );
                mGlyphPositionsProp.emplace( nGlyph, std::make_pair<olc::vi2d, olc::vi2d>( {x,y}, {nWidth, nCharHeight-1}) );
            }

            fCharWidth = float(nCharWidth);
            fCharHeight = float(nCharHeight);
        }
        else
        {
            // No "CFNT" signature found - hint to std out
            printf( "%s is not a valid custom font\n", sFontFile.c_str() );
        }
    }


    olc::vi2d CustomFont::GetTextSize(const std::wstring& s)
	{
		olc::vi2d size = { 0,1 };
		olc::vi2d pos = { 0,1 };
		for (auto c : s)
		{
			if (c == '\n') { pos.y++;  pos.x = 0; }
			else pos.x++;
			size.x = std::max(size.x, pos.x);
			size.y = std::max(size.y, pos.y);
		}
		return size * vf2d( fCharWidth, fCharHeight );
	}

    
	olc::vi2d CustomFont::GetTextSizeProp(const std::wstring& s)
	{
		olc::vi2d size = { 0,1 };
		olc::vi2d pos = { 0,1 };
		for (auto c : s)
		{
			if (c == '\n') { pos.y += 1 ;  pos.x = 0; }
			else pos.x += mGlyphPositionsProp[c].second.x;
            size.x = std::max(size.x, pos.x);
			size.y = std::max(size.y, pos.y);
		}

		size.y *= (int)fCharHeight;
		return size;
	}
    

	void CustomFont::DrawStringDecal(const olc::vf2d& pos, const std::wstring& sText, const Pixel col, const olc::vf2d& scale)
	{
		olc::vf2d spos = { 0.0f, 0.0f };
		for (auto c : sText)
		{
			if (c == '\n')
			{
				spos.x = 0; spos.y += fCharHeight * scale.y;
			}
			else
			{
                auto& glyph = mGlyphPositionsMono[c];
				pge->DrawPartialDecal(pos + spos, fontDecal.get(), glyph.first, glyph.second, scale, col);
				spos.x += fCharWidth * scale.x;
			}
		}
	}

    
	void CustomFont::DrawRotatedStringDecal(const olc::vf2d& pos, const std::wstring& sText, const float fAngle, const olc::vf2d& center, const Pixel col, const olc::vf2d& scale )
    {
		olc::vf2d spos = center;
		for (auto c : sText)
		{
			if (c == '\n')
			{
				spos.x = center.x; spos.y -= fCharHeight;

			}
			else
			{
                auto& glyph = mGlyphPositionsMono[c];
				pge->DrawPartialRotatedDecal(pos, fontDecal.get(), fAngle, spos, glyph.first, glyph.second, scale, col);
				spos.x -= fCharWidth;
			}
		}
    }


	void CustomFont::DrawStringPropDecal(const olc::vf2d& pos, const std::wstring& sText, const Pixel col, const olc::vf2d& scale)
	{
		olc::vf2d spos = { 0.0f, 0.0f };
		for (auto c : sText)
		{
			if (c == '\n')
			{
				spos.x = 0; spos.y += fCharHeight * scale.y;
			}
			else
			{
                auto& glyph = mGlyphPositionsProp[c];
				pge->DrawPartialDecal(pos + spos, fontDecal.get(), glyph.first, glyph.second, scale, col);
				spos.x += glyph.second.x * scale.x;
			}
		}
	}


	void CustomFont::DrawRotatedStringPropDecal(const olc::vf2d& pos, const std::wstring& sText, const float fAngle, const olc::vf2d& center, const Pixel col, const olc::vf2d& scale )
    {
		olc::vf2d spos = center;

		for (auto c : sText)
		{
			if (c == '\n')
			{
				spos.x = center.x; spos.y -= fCharHeight;
			}
			else
			{
                auto& glyph = mGlyphPositionsProp[c];
				pge->DrawPartialRotatedDecal(pos, fontDecal.get(), fAngle, spos, glyph.first, glyph.second, scale, col);
				spos.x -= glyph.second.x;
			}
		}
    }
}
 
#endif
#endif
