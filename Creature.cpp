#include "Creature.h"
#include "Globals.h"
#include "Map.h"
#include "Monster.h"
#include "Player.h"

Creature::Creature()
{
	m_isAttacked = false;
}

void Creature::Move( direction_t direction )
{
	bool canMove = true;
	Position newPosition = GetPosition();
	std::vector<Monster*>& monsters = Globals::currentMap->GetMonstersVector();
	std::vector<StaticMapItem*>&	staticMapItems = Globals::currentMap->GetStaticMapItemVector();

	switch( direction )
	{
	case NORTH:
		newPosition.y--;
		break;

	case SOUTH:
		newPosition.y++;
		break;

	case EAST:
		newPosition.x++;
		break;

	case WEST:
		newPosition.x--;
		break;
	}

	for( std::vector<Monster*>::const_iterator cit = monsters.begin() ; cit != monsters.end() ; ++cit )
	{
		if( (*cit)->GetPosition().x == newPosition.x && ( *cit )->GetPosition().y == newPosition.y )
			canMove = false;
	}

	for( std::vector<StaticMapItem*>::const_iterator cit = staticMapItems.begin() ; cit != staticMapItems.end() ; ++cit )
	{
		if( ( *cit )->GetPosition().x == newPosition.x && ( *cit)->GetPosition().y == newPosition.y )
		{
			if( !(*cit)->IsWalkable() )
				canMove = false;

			return;
		}
	}

	if( newPosition.x == Globals::player->GetPosition().x && newPosition.y == Globals::player->GetPosition().y )
	{
		canMove = false;
	}

	if( canMove )
		SetPosition( newPosition );
}

void Creature::Heal( int healPower )
{
	m_healthPoints += healPower;
	if( m_healthPoints > m_maxHealthPoints )
	{
		m_healthPoints = m_maxHealthPoints;
	}
}