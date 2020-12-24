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

#include <iostream>
#include <sstream>

Monster::Monster()
{
	Globals::spriteManager->GetSprite( m_attackingPlayerSprite, "attacked02" );
	Globals::spriteManager->GetSprite( m_attackedByPlayerSprite, "attacked01" );
	Globals::spriteManager->GetSprite( m_healthBarSprite, "healthbar" );

	m_healthPointsFont = new BitmapFont( FontStyle::FONT_WHITE_SMALL );
}

void Monster::Think( void )
{
	RandomMovement();
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

	/*std::ostringstream sshp;
	sshp << "HP " << m_healthPoints << "/" << m_maxHealthPoints;
	m_healthPointsFont->show_text( static_cast<int>( flposition_x - 5 - Globals::camera->GetCameraX() ), static_cast<int>( flposition_y - 5 - Globals::camera->GetCameraY() ), sshp.str(), Globals::screen );*/

	GameObject::Draw();
	DrawHealthBar();
}

void Monster::RandomMovement( void )
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

void Monster::AttackPlayer( void )
{
	Player* player = Globals::player;
	if( static_cast<int>( Tools::CalculateDistance( player->GetPosition().x, player->GetPosition().y, m_position.x, m_position.y ) ) == 1 )
	{
		m_attackingPlayer = true; 

		if( Globals::currentTime >= m_nextAttack )
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

void Monster::DrawHealthBar( void )
{
	DrawHealthBarWhenAttackedByPlayer( GetSDLRectangleForMonsterHealthBar() );
	DrawHealthBarOnMouseOver( GetSDLRectangleForMonsterHealthBar() );
}

SDL_Rect* Monster::GetSDLRectangleForMonsterHealthBar( void )
{
	SDL_Rect rect[2];
	rect[0].x = 0;
	rect[0].y = 5;
	rect[0].w = ( GetHealthPoints() * 100 / GetMaxHealthPoints() ) * 50 / 100;
	rect[0].h = 5;

	rect[1].x = 0;
	rect[1].y = 0;
	rect[1].w = 50;
	rect[1].h = 5;

	return rect;
}

void Monster::DrawHealthBarWhenAttackedByPlayer( SDL_Rect *rect )
{
	if( m_attackedByPlayer )
	{
		m_healthBarSprite.Draw( Globals::screen, static_cast<int>( flposition_x - Globals::camera->GetCameraX() ), static_cast<int>( flposition_y - 10 - Globals::camera->GetCameraY() ), &rect[1] );
		m_healthBarSprite.Draw( Globals::screen, static_cast<int>( flposition_x - Globals::camera->GetCameraX() ), static_cast<int>( flposition_y - 10 - Globals::camera->GetCameraY() ), &rect[0] );
	}
}

void Monster::DrawHealthBarOnMouseOver( SDL_Rect* rect )
{
	Position mouse;
	mouse.x = ( Globals::event.motion.x + Globals::camera->GetCameraX() ) / Globals::tilesize;
	mouse.y = ( Globals::event.motion.y + Globals::camera->GetCameraY() ) / Globals::tilesize;

	if( mouse.x == GetPosition().x && mouse.y == GetPosition().y )
	{
		if( !m_attackedByPlayer )
		{
			m_attackingPlayerSprite.Draw( Globals::screen, static_cast<int>( flposition_x - Globals::camera->GetCameraX() ), static_cast<int>( flposition_y - Globals::camera->GetCameraY() ) );
		}

		m_healthBarSprite.Draw( Globals::screen, static_cast<int>( flposition_x - Globals::camera->GetCameraX() ), static_cast<int>( flposition_y - 10 - Globals::camera->GetCameraY() ), &rect[1] );
		m_healthBarSprite.Draw( Globals::screen, static_cast<int>( flposition_x - Globals::camera->GetCameraX() ), static_cast<int>( flposition_y - 10 - Globals::camera->GetCameraY() ), &rect[0] );
	}
}