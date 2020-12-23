#include "FogOfWar.h"

#include "Camera.h"
#include "Game.h"
#include "GameState.h"
#include "Globals.h"
#include "Player.h"
#include "SpriteManager.h"
#include "Tools.h"

FogOfWar::FogOfWar()
{
	m_explored = false;
}

void FogOfWar::Think( void )
{
	if( Globals::game->GetGameStateEnum() == MAPEDITOR )
		return;

	Player* player = Globals::player;
	int distance = static_cast<int>( Tools::CalculateDistance( m_position.x, m_position.y, player->GetPosition().x, player->GetPosition().y ) );

	if( distance <= FOGOFWAR_DISTANCE )
	{
		m_explored = true;
	}

	else
	{
		m_explored = false;
	}
}


void FogOfWar::Draw( void ) //Nie uzywana funkcja. Poniewaz zamula gre.
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



