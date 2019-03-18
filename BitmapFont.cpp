#include "BitmapFont.h"

SDL_Surface* BitmapFont::load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

void BitmapFont::apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}


Uint32 get_pixel32( int x, int y, SDL_Surface *surface )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;

    //Get the pixel requested
    return pixels[ ( y * surface->w ) + x ];
}

BitmapFont::BitmapFont( FontStyle::FONTCOLORSIZE fontStyle )
{
	newLine = 0;
    space = 0;

	switch( fontStyle )
	{
	case FontStyle::FONT_WHITE_BIG:
		bitmapFont = load_image( "pliki/grafika/bitmapfonts/font_big_white.png" );
		break;

	case FontStyle::FONT_WHITE_SMALL:
		bitmapFont = load_image( "pliki/grafika/bitmapfonts/font_small_white.png" );
		break;

	case FontStyle::FONT_WHITE_SMALL_OLD:
		bitmapFont = load_image( "pliki/grafika/bitmapfonts/font_small_white_old.png" );
		break;
	}

    //Build the font
    build_font( bitmapFont );
}

void BitmapFont::build_font( SDL_Surface *surface )
{
    //If surface is NULL
    if( surface == NULL )
    {
        return;
    }

    //Get the bitmap
    bitmap = surface;

    //Set the background color
    Uint32 bgColor = SDL_MapRGB( bitmap->format, 0, 0xFF, 0xFF );

    //Set the cell dimensions
    int cellW = bitmap->w / 16;
    int cellH = bitmap->h / 16;

    //New line variables
    int top = cellH;
    int baseA = cellH;

    //The current character we're setting
    int currentChar = 0;

    //Go through the cell rows
    for( int rows = 0; rows < 16; rows++ )
    {
        //Go through the cell columns
        for( int cols = 0; cols < 16; cols++ )
        {
            //Set the character offset
            chars[ currentChar ].x = cellW * cols;
            chars[ currentChar ].y = cellH * rows;

            //Set the dimensions of the character
            chars[ currentChar ].w = cellW;
            chars[ currentChar ].h = cellH;

            //Find Left Side
            //Go through pixel columns
            for( int pCol = 0; pCol < cellW; pCol++ )
            {
                //Go through pixel rows
                for( int pRow = 0; pRow < cellH; pRow++ )
                {
                    //Get the pixel offsets
                    int pX = ( cellW * cols ) + pCol;
                    int pY = ( cellH * rows ) + pRow;

                    //If a non colorkey pixel is found
                    if( get_pixel32( pX, pY, bitmap ) != bgColor )
                    {
                        //Set the x offset
                        chars[ currentChar ].x = pX;

                        //Break the loops
                        pCol = cellW;
                        pRow = cellH;
                    }
                }
            }

            //Find Right Side
            //Go through pixel columns
            for( int pCol_w = cellW - 1; pCol_w >= 0; pCol_w-- )
            {
                //Go through pixel rows
                for( int pRow_w = 0; pRow_w < cellH; pRow_w++ )
                {
                    //Get the pixel offsets
                    int pX = ( cellW * cols ) + pCol_w;
                    int pY = ( cellH * rows ) + pRow_w;

                    //If a non colorkey pixel is found
                    if( get_pixel32( pX, pY, bitmap ) != bgColor )
                    {
                        //Set the width
                        chars[ currentChar ].w = ( pX - chars[ currentChar ].x ) + 1;

                        //Break the loops
                        pCol_w = -1;
                        pRow_w = cellH;
                    }
                }
            }

            //Find Top
            //Go through pixel rows
            for( int pRow = 0; pRow < cellH; pRow++ )
            {
                //Go through pixel columns
                for( int pCol = 0; pCol < cellW; pCol++ )
                {
                    //Get the pixel offsets
                    int pX = ( cellW * cols ) + pCol;
                    int pY = ( cellH * rows ) + pRow;

                    //If a non colorkey pixel is found
                    if( get_pixel32( pX, pY, bitmap ) != bgColor )
                    {
                        //If new top is found
                        if( pRow < top )
                        {
                            top = pRow;
                        }

                        //Break the loops
                        pCol = cellW;
                        pRow = cellH;
                    }
                }
            }

            //Find Bottom of A
            if( currentChar == 'A' )
            {
                //Go through pixel rows
                for( int pRow = cellH - 1; pRow >= 0; pRow-- )
                {
                    //Go through pixel columns
                    for( int pCol = 0; pCol < cellW; pCol++ )
                    {
                        //Get the pixel offsets
                        int pX = ( cellW * cols ) + pCol;
                        int pY = ( cellH * rows ) + pRow;

                        //If a non colorkey pixel is found
                        if( get_pixel32( pX, pY, bitmap ) != bgColor )
                        {
                            //Bottom of a is found
                            baseA = pRow;

                            //Break the loops
                            pCol = cellW;
                            pRow = -1;
                        }
                    }
                }
            }

            //Go to the next character
            currentChar++;
        }
    }

    //Calculate space
    space = cellW / 2;

    //Calculate new line
    newLine = baseA - top;

    //Lop off excess top pixels
    for( int t = 0; t < 256; t++ )
    {
        chars[ t ].y += top;
        chars[ t ].h -= top;
    }
}

void BitmapFont::show_text( int x, int y, std::string text, SDL_Surface *surface )
{
    //Temp offsets
    int X = x, Y = y;

    //If the font has been built
    if( bitmap != NULL )
    {
        //Go through the text
        for( int show = 0; show < text.length(); show++ )
        {
            //If the current character is a space
            if( text[ show ] == ' ' )
            {
                //Move over
                X += space;
            }
            //If the current character is a newline
            else if( text[ show ] == '\n' )
            {
                //Move down
                Y += newLine + 5;

                //Move back
                X = x;
            }
            else
            {
                //Get the ASCII value of the character
                int ascii = (unsigned char)text[ show ];

                //Show the character
                apply_surface( X, Y, bitmap, surface, &chars[ ascii ] );

                //Move over the width of the character with one pixel of padding
                X += chars[ ascii ].w + 1;
            }
        }
    }
}