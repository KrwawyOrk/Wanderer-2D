#include "Monster.h"

#include "BitmapFont.h"
#include "Camera.h"
#include "Container.h"
#include "Factory.h"
#include "Globals.h"
#include "Item.h"
#include "Map.h"
#include "Player.h"
#include "SpriteManager.h"
#include "Tools.h"
#include "LootGenerator.h"
#include "MonsterHealthBar.h"

#include <iostream>
#include <sstream>
#include <cmath>
#include "SDL.h"

Monster::Monster()
{
	Globals::spriteManager->GetSprite( m_attackingPlayerSprite, "attacked02" );
	Globals::spriteManager->GetSprite( m_attackedByPlayerSprite, "attacked01" );
	Globals::spriteManager->GetSprite( m_healthBarSprite, "healthbar" );

	m_monsterHealthBar = new MonsterHealthBar( this );
	m_healthPointsFont = new BitmapFont( FontStyle::FONT_WHITE_SMALL );
}

Monster::~Monster()
{
	delete m_monsterHealthBar;
}

void Monster::Think( void )
{
	//RandomMovement();
	MoveTowardsPlayer();
	AttackPlayer();
}

void Monster::Draw( void )
{
	if( m_attackingPlayer && !m_attackedByPlayer )
	{
		m_attackingPlayerSprite.Draw( Globals::screen, static_cast<int>( flposition_x - Globals::camera->GetCameraX() ), static_cast<int>( flposition_y - Globals::camera->GetCameraY() ) );
	}

	if( m_attackedByPlayer )
	{
		m_attackedByPlayerSprite.DrawAnimatedOnScreenPosition( Globals::screen, static_cast<int>( flposition_x - Globals::camera->GetCameraX() ), static_cast<int>( flposition_y - Globals::camera->GetCameraY() ), 100 );
	}

	GameObject::Draw();
	m_monsterHealthBar->Draw();
}

void Monster::Die( void )
{
	Player* player = Globals::player;
	player->GiveExperiencePoints( 100 );

	std::cout << "Potworek zwany " << this->GetName() << " zostal zabity! :)";
	std::cout << "Experience points: " << player->GetExperiencePoints() << std::endl;
}

bool Monster::CanRandomMovement( void )
{
	int distance = Tools::CalculateDistance( Globals::player->GetPosition().x, Globals::player->GetPosition().y, GetPosition().x, GetPosition().y );

	if( distance < 6 )
	{
		return true;
	}

	else
	{
		return false;
	}
}

void Monster::RandomMovement( void )
{
	if( CanRandomMovement () )
	{
		if( !IsMoving() && !m_attackingPlayer && Globals::currentTime > m_nextMove )
		{
			int min = 1;
			int max = 4;
			int random = min + ( rand() % static_cast<int>( max - min + 1 ) );

			switch( random )
			{
			case 1:
				Move( NORTH );
				break;

			case 2:
				Move( SOUTH );
				break;

			case 3:
				Move( WEST );
				break;

			case 4:
				Move( EAST );
				break;
			};

			m_nextMove = Globals::currentTime + 0.0f;
		}
	}
}

void Monster::AttackPlayer( void )
{
	Player* player = Globals::player;
	if( IsPlayerInAttackDistance( 1, player ) )
	{
		m_attackingPlayer = true; 

		if( IsCooldownReadyToAttack() )
		{
			player->SetHealthPoints( player->GetHealthPoints() - m_baseDamage );
			m_nextAttack = Globals::currentTime + MONSTER_ATTACK_DELAY;
		}
	}

	else
	{
		m_attackingPlayer = false;
	}
}

void Monster::CreateCorpseWithLootItems( void )
{
	StaticMapItem* item = Globals::factory->CreateStaticMapItem( GetPosition(), "deadspider", true, "spiderdeadbody" );

	Container* container = new Container;
	//container->AddItem( new Item( Position( 5, 5 ), HEALTH_REGENERATION ) );
	LootGenerator lootGenerator;
	lootGenerator.FillContainerWithRandomLoot( container );

	item->SetContainer( container );
	
	Map* map = Globals::currentMap;
	map->GetStaticMapItemVector().push_back( item );
}

bool Monster::IsCursorOnMonster( void )
{
	int mouse_x = Globals::event.motion.x;
	int mouse_y = Globals::event.motion.y;

	int position_x = static_cast<int>( this->GetFloatX() - Globals::camera->GetCameraX() );
	int position_y = static_cast<int>( this->GetFloatY() - Globals::camera->GetCameraY() );

	return ( mouse_x >= position_x && mouse_x <= position_x + Globals::tilesize && mouse_y >= position_y && mouse_y <= position_y + Globals::tilesize );
}

bool Monster::IsCooldownReadyToAttack( void )
{
	return (Globals::currentTime >= m_nextAttack);
}

bool Monster::IsPlayerInAttackDistance( int distance, Player* player )
{
	return (static_cast<int>(Tools::CalculateDistance( player->GetPosition().x, player->GetPosition().y, m_position.x, m_position.y )) <= distance);
}

void Monster::MoveTowardsPlayer( void ) 
{
	double radius = std::sqrt( std::pow( m_position.x - Globals::player->GetPosition().x, 2 ) + std::pow( m_position.y - Globals::player->GetPosition().y, 2 ) );

	if (!IsMoving() && !m_attackingPlayer && Globals::currentTime > m_nextMove)
	{
		if (radius < 7) {
			if (m_position.x < Globals::player->GetPosition().x) {
				Move( EAST );
			}

			else if (m_position.x > Globals::player->GetPosition().x) {	
				Move( WEST );
			}

			else if (m_position.y < Globals::player->GetPosition().y) {	
				Move( SOUTH );
			}

			else if (m_position.y > Globals::player->GetPosition().y) {		
				Move( NORTH );
			}

			m_nextMove = Globals::currentTime + 0.0f;
		}
	}
}