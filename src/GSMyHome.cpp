#include "GSMyHome.h"

#include "Button.h"
#include "Game.h"
#include "Globals.h"
#include "GSPlaying.h"
#include "Player.h"
#include "SpriteManager.h"

GSMyHome::GSMyHome() 
{
	m_homeState = HomeState::HOME;
	Globals::spriteManager->GetSprite( m_background, "my_home_background" );
	m_buttonComputer = new Button( 30, 305, "my_home_button_computer", true );
}

GSMyHome::~GSMyHome()
{
	delete m_buttonComputer;
	m_buttonComputer = NULL;
}

void GSMyHome::InputEvents( void )
{
	if( Globals::event.type == SDL_KEYDOWN && Globals::event.key.keysym.sym == SDLK_ESCAPE )
	{
		Globals::game->SetGameState( "Play" );
		Globals::game->FadeToBlack( Globals::screen, 1000 );
	}

	if (m_buttonComputer->ButtonClicked())
	{	
		Globals::game->SetGameState( "Play" );
		Globals::game->FadeToBlack( Globals::screen, 1000 );
	}
}

void GSMyHome::Think( void )
{
	//TODO
}

void GSMyHome::Update( float deltaTime )
{
	//TODO
}

void GSMyHome::Draw( void )
{
	m_background.Draw( Globals::screen, 0, 0 );
	m_buttonComputer->DrawButton();
}

void GSMyHome::OnQuit( void )
{
	//TODO
}