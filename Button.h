#ifndef BUTTON_H
#define BUTTON_H

#include "SDL.h"

#include "Sprite.h"

class Button
{
public:
	Button( std::string spriteName, bool hoverEffect = false );
	Button( int pos_x, int pos_y, std::string spriteName, bool hoverEffect = false );

	bool ButtonClicked( void );
	bool ButtonHover( void );
	void DrawButton( void );

	//Function used only in sprite browser of map editor. Other drawing functions cut the width of the picture to 50 px.
	void DrawButtonInSpriteBrowser( void );

	Sprite& GetSprite( void )
	{
		return m_sprite;
	}

	int position_x;
	int position_y;

	private:
	Sprite m_sprite;
	bool m_hoverEffect;
};

#endif

