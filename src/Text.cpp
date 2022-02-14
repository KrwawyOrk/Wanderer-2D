#include "Text.h"

#include "Globals.h"

Text::Text( int fontSize, textColor color, Position position )
{
	m_text = "no text";
	m_surface = NULL;

	m_position.x = position.x;
	m_position.y = position.y;

	SetTextColor( color );

	m_font = TTF_OpenFont( "pliki/font.ttf", fontSize );
}

Text::~Text()
{
	SDL_FreeSurface( m_surface );
	TTF_CloseFont( m_font );
}

void Text::DrawText( SDL_Surface* screen )
{
	if( m_surface )
	{
		SDL_Rect rect;
		rect.x = m_position.x;
		rect.y = m_position.y;

		SDL_BlitSurface( m_surface, NULL, Globals::screen, &rect );
	}
}

void Text::Update( void )
{
	m_surface = TTF_RenderText_Solid( m_font, m_text.c_str(), m_color );
}

void Text::SetTextColor( textColor color )
{
	switch( color )
	{
		case WHITE:
			m_color.r = 255;
			m_color.g = 255;
			m_color.b = 255;
			break;

		case GOLD:
			m_color.r = 246;
			m_color.g = 164;
			m_color.b = 5;
			break;

		case GREEN:
			m_color.r = 55;
			m_color.g = 145;
			m_color.b = 0;
			break;
	}
}



