#include "Button.h"

#include "Globals.h"
#include "SpriteManager.h"

#include "SDL.h"

Button::Button( std::string spriteName, bool hoverEffect )
{
	position_x = 0;
	position_y = 0;
	m_hoverEffect = hoverEffect;

	Globals::spriteManager->GetSprite( m_sprite, spriteName );
}

Button::Button( int pos_x, int pos_y, std::string spriteName, bool hoverEffect )
{
	position_x = pos_x;
	position_y = pos_y;
	m_hoverEffect = hoverEffect;

	Globals::spriteManager->GetSprite( m_sprite, spriteName );
}

void Button::DrawButton( void )
{
	if( ButtonHover() && m_hoverEffect )
	{

		SDL_Rect rect;
		rect.x = 0;
		rect.y = m_sprite.GetSDLSurface()->h / 2;
		rect.w = m_sprite.GetSDLSurface()->w;
		rect.h = m_sprite.GetSDLSurface()->h / 2;

		m_sprite.Draw( Globals::screen, position_x, position_y, rect );
	}

	else if( !ButtonHover() && m_hoverEffect )
	{
		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = m_sprite.GetSDLSurface()->w;
		rect.h = m_sprite.GetSDLSurface()->h / 2;

		m_sprite.Draw( Globals::screen, position_x, position_y, rect );
	}

	if( !m_hoverEffect )
	{
		m_sprite.Draw( Globals::screen, position_x, position_y );
	}
}

bool Button::ButtonClicked( void )
{
	bool buttonclicked = false;
	int mouse_x = Globals::event.button.x;
	int mouse_y = Globals::event.button.y;

	SDL_Rect& rect = m_sprite.GetSDLSurface()->clip_rect;

	if( Globals::event.button.button == SDL_BUTTON_LEFT )
	{
		if( !m_hoverEffect )
		{
			if( mouse_x >= position_x && mouse_x <= position_x + rect.w && mouse_y >= position_y && mouse_y <= position_y + rect.h )
			{
				buttonclicked = true;
			}
		}

		else
		{
			if( mouse_x >= position_x && mouse_x <= position_x + rect.w && mouse_y >= position_y && mouse_y <= position_y + rect.h / 2)
			{
				buttonclicked = true;
			}
		}
	}

	return buttonclicked;
}

bool Button::ButtonHover( void )
{
	bool buttonHover = false;
	int mouse_x = Globals::event.motion.x;
	int mouse_y = Globals::event.motion.y;

	SDL_Rect& rect = m_sprite.GetSDLSurface()->clip_rect;

	if( !m_hoverEffect )
	{
		if( mouse_x >= position_x && mouse_x <= position_x + rect.w && mouse_y >= position_y && mouse_y <= position_y + rect.h )
		{
			buttonHover = true;
		}
	}

	else
	{
		if( mouse_x >= position_x && mouse_x <= position_x + rect.w && mouse_y >= position_y && mouse_y <= position_y + rect.h / 2)
		{
			buttonHover = true;
		}
	}

	return buttonHover;
}