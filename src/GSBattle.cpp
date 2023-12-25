#include "GSBattle.h"

#include "Creature.h"
#include "Game.h"
#include "Globals.h"
#include "GSTest.h"
#include "Player.h"
#include "SpriteManager.h"

void GSBattle::LoadFiles( void )
{
	Globals::spriteManager->GetSprite( m_background, "battlebackground" );
}

void GSBattle::InputEvents( void )
{
	if( Globals::event.type == SDL_KEYDOWN )
	{
		switch( Globals::event.key.keysym.sym )
		{
		case SDLK_ESCAPE:
			Globals::game->SetGameState( "Test" );
			break;
		}
	}
}

void GSBattle::Draw( void )
{
	//m_background.Draw();
}

void GSBattle::StartBattle( Player* player, Creature* attackedCreature )
{
	return;
}
