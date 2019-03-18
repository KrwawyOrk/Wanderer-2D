#include "GUITextInput.h"
#include "Globals.h"
#include "SpriteManager.h"

#include <sstream>

GUITextInput::GUITextInput( Position position, bool hidePassword )
{
	m_text = "";
	m_surface = NULL;
	m_font = TTF_OpenFont( "pliki/font.ttf", 20 );

	m_inputEnabled = false;
	m_hidePassword = hidePassword;

	m_labelPosition.x = position.x;
	m_labelPosition.y = position.y;

	Globals::spriteManager->GetSprite( m_sprite, "textinput" );

	SDL_Color color = { 174, 0, 0 };

	m_surface = TTF_RenderText_Solid( m_font, m_text.c_str(), color );
	SDL_EnableUNICODE( SDL_ENABLE );
}

GUITextInput::~GUITextInput()
{
	SDL_FreeSurface( m_surface );
	TTF_CloseFont( m_font );
	SDL_EnableUNICODE( SDL_DISABLE );
}

void GUITextInput::InputEvents( void )
{
	if( Globals::event.type == SDL_MOUSEBUTTONDOWN )
	{
		if( Globals::event.button.button == SDL_BUTTON_LEFT )
		{
			Position mouse( Globals::event.motion.x, Globals::event.motion.y );

			if( mouse.x >= m_labelPosition.x && mouse.x <= m_labelPosition.x + 200 && mouse.y >= m_labelPosition.y && mouse.y <= m_labelPosition.y + 30 )
			{
				if( !m_inputEnabled )
					m_inputEnabled = true;
			}

			else
			{
				if( m_inputEnabled )
					m_inputEnabled = false;
			}
		}
	}

	if( m_inputEnabled )
	{
		if( Globals::event.type == SDL_KEYDOWN )
		{
			if( Globals::event.key.keysym.sym == SDLK_ESCAPE )
			{
				Clear();
				DisableInput();
			}

			std::string temp = m_text;

			if( m_text.length() < 16 )
			{
				if( ( Globals::event.key.keysym.unicode >= (Uint16)'0' && Globals::event.key.keysym.unicode <= (Uint16)'9' ) )
					m_text += (char)Globals::event.key.keysym.unicode;

				if( ( Globals::event.key.keysym.unicode >= (Uint16)'a' && Globals::event.key.keysym.unicode <= (Uint16)'z' ) )
					m_text += (char)Globals::event.key.keysym.unicode;
			}

			if( Globals::event.key.keysym.sym == SDLK_BACKSPACE && m_text.length() != 0 )
				m_text.erase( m_text.length() - 1 );

			if( m_text != temp )
			{
				SDL_FreeSurface( m_surface );

				SDL_Color color = { 174, 0, 0 };
				m_surface = TTF_RenderText_Solid( m_font, m_text.c_str(), color );
			}
		}
	}
}

void GUITextInput::Draw( void )
{
	m_sprite.Draw( Globals::screen, m_labelPosition.x, m_labelPosition.y );

	if( m_surface )
	{
		SDL_Rect rect;
		rect.x = m_labelPosition.x;
		rect.y = m_labelPosition.y;

		SDL_BlitSurface( m_surface, NULL, Globals::screen, &rect );
	}
}

void GUITextInput::Clear( void )
{
	m_text.clear();
	SDL_Color color = { 174, 0, 0 };
	m_surface = TTF_RenderText_Solid( m_font, m_text.c_str(), color );
}

int GUITextInput::GetTextToInt( void )
{
	std::istringstream iss( GetText() );

	int i = 0;
	iss >> i;

	return i;
}
