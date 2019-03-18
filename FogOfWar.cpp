#include "FogOfWar.h"

#include "Camera.h"
#include "Game.h"
#include "GameState.h"
#include "Globals.h"
#include "Player.h"
#include "SpriteManager.h"

FogOfWar::FogOfWar()
{
	m_explored = false;
}

void FogOfWar::Think( void )
{
	if( Globals::game->GetGameStateEnum() == MAPEDITOR )
		return;

	if( !IsExplored() )
	{
	    Player* player = Globals::player;
		if( player->GetPosition().x >= m_position.x - FOGOFWAR_DISTANCE && player->GetPosition().x <= m_position.x + FOGOFWAR_DISTANCE && player->GetPosition().y >= m_position.y - FOGOFWAR_DISTANCE && player->GetPosition().y <= m_position.y + FOGOFWAR_DISTANCE )
		{
			m_explored = true;
		}
	}
}

void FogOfWar::Draw( void )
{
	if( Globals::game->GetGameStateEnum() == MAPEDITOR )
		return;

	if( !IsExplored() )
	{
		int x = m_position.x * Globals::tilesize - Globals::camera->GetCameraX();
		int y = m_position.y * Globals::tilesize - Globals::camera->GetCameraY();

		m_sprite.Draw( Globals::screen, x, y );
	}
}



