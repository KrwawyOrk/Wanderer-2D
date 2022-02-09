#ifndef TEXT_H
#define TEXT_H

#include "Position.h"
#include "Sprite.h"

#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

enum textColor
{
	WHITE,
	GOLD,
	GREEN
};

class Text
{
public:
	Text( int fontSize, textColor color, Position position );
	~Text();

	std::string GetText( void ) const { return m_text; }
	void SetText( std::string text )
	{
		m_text = text;
	}

	Position& GetPosition( void ) { return m_position; }
	void SetPosition( int x, int y )
	{
		m_position.x = x;
		m_position.y = y;
	}

	void DrawText( SDL_Surface* screen );
	void Update( void ); // Ta funkcja musi zostaæ wywo³ana po ka¿dej zmianie w obiekcie Text.
	void SetTextColor( textColor color );

private:
	std::string m_text;
	Position m_position;
	textColor m_textColor;

	SDL_Color m_color;
	TTF_Font* m_font;
	SDL_Surface* m_surface;
};

#endif
