#include "GSMap.h"

#include "Camera.h"
#include "Game.h"
#include "Globals.h"
#include "SpriteManager.h"

GSMap::GSMap(void)
{
	Globals::spriteManager->GetSprite( m_map, "worldmap" );
}


GSMap::~GSMap(void)
{
}

void GSMap::Draw( void )
{
	m_map.Draw( Globals::screen, 0, 0 );
}

void GSMap::InputEvents( void )
{
	if (Globals::event.type == SDL_KEYDOWN && Globals::event.key.keysym.sym == SDLK_ESCAPE)
	{
		Globals::game->SetGameState( "Play" );
		Globals::camera->FollowPlayer( true );
	}
}


