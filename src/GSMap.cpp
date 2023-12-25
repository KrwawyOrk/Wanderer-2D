#include "GSMap.h"

#include "Button.h"
#include "Camera.h"
#include "Game.h"
#include "Globals.h"
#include "SpriteManager.h"
#include "GSPlaying.h"

GSMap::GSMap(void)
{
	Globals::spriteManager->GetSprite( m_map, "worldmap" );
	m_exitButton = new Button( 1285, 115, "close_button", true );
	m_secretLaboratory = new Button( 400, 500, "secret_laboratory_button", false );
}


GSMap::~GSMap(void)
{
	delete m_exitButton;
	delete m_secretLaboratory;

}

void GSMap::Draw( void )
{
	m_map.Draw( Globals::screen, 0, 0 );
	m_exitButton->DrawButton();
	m_secretLaboratory->DrawButton();
}

void GSMap::InputEvents( void )
{
	if (Globals::event.type == SDL_KEYDOWN && Globals::event.key.keysym.sym == SDLK_ESCAPE)
	{
		Globals::game->SetGameState( "Play" );
		Globals::camera->FollowPlayer( true );
	}
}

void GSMap::Think( void )
{
	if (m_exitButton->ButtonClicked())
	{
		Globals::game->SetGameState( "Play" );
		Globals::camera->FollowPlayer( true );
	}

	if (m_secretLaboratory->ButtonClicked())
	{
		GSPlaying* gsPlaying = dynamic_cast<GSPlaying*>(Globals::game->GetGameState( "Play" ));
		gsPlaying->ChangeMap( "lasek" );
		Globals::game->SetGameState( "Play" );
	}
}


