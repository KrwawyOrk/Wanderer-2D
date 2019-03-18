/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//Thank you Lazy Foo for that piece of code. It helped me a lot :)

#ifndef BITMAPFONT_H
#define BITMAPFONT_H

#include "SDL.h"
#include "SDL_image.h"
#include <string>

namespace FontStyle
{
	enum FONTCOLORSIZE
	{
		FONT_WHITE_SMALL = 1,
		FONT_WHITE_BIG,
		FONT_WHITE_SMALL_OLD
	};
}

class BitmapFont
{
    public:
    //The default constructor
    BitmapFont( FontStyle::FONTCOLORSIZE fontStyle );

    //Shows the text
    void show_text( int x, int y, std::string text, SDL_Surface *surface );

	private:
	void build_font( SDL_Surface *surface );
	SDL_Surface* load_image( std::string filename );
	void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );
    //The font surface
    SDL_Surface *bitmap;
	SDL_Surface* bitmapFont;

    //The individual characters in the surface
    SDL_Rect chars[ 256 ];

    //Spacing Variables
    int newLine, space;
};

#endif