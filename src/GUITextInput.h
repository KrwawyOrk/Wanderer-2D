#ifndef GUITEXTINPUT_H
#define GUITEXTINPUT_H

#include "Position.h"
#include "Sprite.h"

#include <string>
#include "SDL.h"
#include "SDL_ttf.h"

class GUITextInput
{
public:
	GUITextInput( Position position, bool hidePassword = false );
	~GUITextInput();

	void InputEvents( void );
	void Draw( void );

	void Clear( void );
	std::string GetText( void ) const { return m_text; }
	int GetTextToInt( void );

	bool InputEnabled( void ) const { return m_inputEnabled; }
	void DisableInput( void ) { m_inputEnabled = false; }

private:
	std::string m_text;
	Position m_labelPosition;
	Sprite m_sprite;

	bool m_inputEnabled;
	bool m_hidePassword;

	SDL_Surface* m_surface;
	TTF_Font* m_font;
};

#endif

