#include "Player.h"

#include "ActionMapChange.h"
#include "Box.h"
#include "Camera.h"
#include "Game.h"
#include "Globals.h"
#include "GSPlaying.h"
#include "Item.h"
#include "Map.h"
#include "Monster.h"
#include "SpriteManager.h"
#include "StaticMapItem.h"
#include "Item.h"
#include "Tools.h"

#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include "rapidxml/rapidxml.hpp"

Player::Player()
{
	LoadPlayerDataFromXML( "pliki/player.xml" );
	m_name = "Gracz";

	flposition_x = static_cast<float>( m_position.x * Globals::tilesize );
	flposition_y = static_cast<float>( m_position.y * Globals::tilesize );

	m_velocity = BASE_VELOCITY;

	m_selected = true;
	m_nextAttack = 0.0;
	m_attackSpeed = 1.0;
	m_weaponType = CROWBAR;

	Globals::spriteManager->GetSprite( m_sprite, "playertest" );
	Globals::spriteManager->GetSprite( m_spriteSelected, "championselection" );
	Globals::currentPlayer = this;

	m_skills.m_battle = 1;
	m_skills.m_medicine = 2;

	m_skillsVector.push_back(Skill( skillTypes::INCREASED_SPEED, false, 0 ) );
	m_skillsVector.push_back(Skill( skillTypes::INCREASED_VISIBILITY, false, 0 ) );

	GetSkillOfType( skillTypes::INCREASED_SPEED ).SetLearnedSkill( true );
	GetSkillOfType( skillTypes::INCREASED_VISIBILITY ).SetLearnedSkill( true );

	set_clips();
	//Initialize movement variables
    offSet = 0;
 
    //Initialize animation variables
    frame = 0;
    status = FOO_RIGHT;

	Globals::spriteManager->GetSprite( m_animationSprite, "foo" );
}

void Player::LoadPlayerDataFromXML( const std::string& fileName )
{
	std::ifstream file( fileName );
	std::string xmlString( (std::istreambuf_iterator<char>( file )), std::istreambuf_iterator<char>() );
	rapidxml::xml_document<> doc;
	doc.parse<0>( &xmlString[0] );

	rapidxml::xml_node<>* root = doc.first_node( "player" );
	m_healthPoints = std::stoi( root->first_node( "health" )->value() );
	m_maxHealthPoints = std::stoi( root->first_node( "maxHealth" )->value() );	
	m_experiencePoints = std::stoi( root->first_node( "experiencePoints" )->value() );
	m_monstersKilled = std::stoi( root->first_node( "monstersKilled" )->value() );
	m_pistolAmmunition = std::stoi( root->first_node( "pistolAmmunition" )->value() );
	m_damage = std::stoi( root->first_node( "damage" )->value() );
	m_position.x = std::stoi( root->first_node( "position_x" )->value() );
	m_position.y = std::stoi( root->first_node( "position_y" )->value() );
}

void Player::Think( void )
{
	/*if( GetSkillOfType( skillTypes::INCREASED_SPEED ).SkillIsLearned() )
	{
		m_velocity = BASE_VELOCITY + 50.0f;
	}

	else 
	{
		m_velocity = BASE_VELOCITY;
	}*/

	CheckReductionInSpeedAtLowHealthPoints();

	//Move
    offSet += static_cast<int>(m_velocity);

    //Keep the stick figure in bounds
    if( ( offSet < 0 ) || ( offSet + FOO_WIDTH > SCREEN_WIDTH ) )
    {
        offSet -= static_cast<int>( m_velocity );
    }
}

void Player::Draw( void )
{
	int animationSpeed = int(m_velocity);

	//If Foo is moving left
	if( IsMovingLeft() )
    {
        //Set the animation to left
        status = FOO_LEFT;

        //Move to the next frame in the animation
		//frame = (SDL_GetTicks() / static_cast<int>(m_velocity) ) % 3;
		frame = int(((SDL_GetTicks() / animationSpeed) % 3));
    }
    //If Foo is moving right
    else if( IsMovingRight() )
    {
        //Set the animation to right
        status = FOO_RIGHT;

        //Move to the next frame in the animation
		//frame = (SDL_GetTicks() / static_cast<int>(m_velocity) ) % 3;
		frame = int(((SDL_GetTicks() / animationSpeed) % 3));
    }
	
	else if( IsMovingUp() )
	{
		status = FOO_UP;
		//frame = (SDL_GetTicks() / static_cast<int>(m_velocity) ) % 3;
		frame = int(((SDL_GetTicks() / animationSpeed) % 3));
	}

	else if( IsMovingDown() )
	{
		status = FOO_DOWN;
		//frame = (SDL_GetTicks() / static_cast<int>(m_velocity) ) % 3;
		frame = int(((SDL_GetTicks() / animationSpeed) % 3));
	}

    //If Foo standing
    if( !IsMoving() )
    {
        //Restart the animation
        frame = 1;
    }

    //Loop the animation
    if( frame >= 3 )
    {
        frame = 0;
    }

    //Show the stick figure
    if( status == FOO_RIGHT )
    {
		int x = static_cast<int>( flposition_x - Globals::camera->GetCameraX() - m_sprite.GetSDLSurface()->w + Globals::tilesize );
		int y = static_cast<int>( flposition_y - Globals::camera->GetCameraY() - m_sprite.GetSDLSurface()->h + Globals::tilesize );
        m_animationSprite.Draw( Globals::screen, x, y, &clipsRight[frame] );
    }

    else if( status == FOO_LEFT )
    {
		int x = static_cast<int>( flposition_x - Globals::camera->GetCameraX() - m_sprite.GetSDLSurface()->w + Globals::tilesize );
		int y = static_cast<int>( flposition_y - Globals::camera->GetCameraY() - m_sprite.GetSDLSurface()->h + Globals::tilesize );
        m_animationSprite.Draw( Globals::screen, x, y, &clipsLeft[frame] );
    }

	else if( status == FOO_UP )
	{
		int x = static_cast<int>( flposition_x - Globals::camera->GetCameraX() - m_sprite.GetSDLSurface()->w + Globals::tilesize );
		int y = static_cast<int>( flposition_y - Globals::camera->GetCameraY() - m_sprite.GetSDLSurface()->h + Globals::tilesize );
        m_animationSprite.Draw( Globals::screen, x, y, &clipsUp[frame] );
	}

	else if( status == FOO_DOWN )
	{
		int x = static_cast<int>( flposition_x - Globals::camera->GetCameraX() - m_sprite.GetSDLSurface()->w + Globals::tilesize );
		int y = static_cast<int>( flposition_y - Globals::camera->GetCameraY() - m_sprite.GetSDLSurface()->h + Globals::tilesize );
        m_animationSprite.Draw( Globals::screen, x, y, &clipsDown[frame] );
	}
}

void Player::Move( direction_t direction )
{
	if( IsMoving() )
	{
		return;
	}

	bool canMove = true;
	Position nextMovePosition = GetPosition();

	switch( direction )
	{
	case NORTH:
		nextMovePosition.y--;
		break;

	case SOUTH:
		nextMovePosition.y++;
		break;

	case EAST:
		nextMovePosition.x++;
		break;

	case WEST:
		nextMovePosition.x--;
		break;
	}

	Map* map = Globals::currentMap;

	for (auto item : map->GetStaticMapItemVector())
	{
		if (item->GetPosition() == nextMovePosition)
		{
			if (!item->IsWalkable())
			{
				canMove = false;
				return;
			}
		}
	}

	for (auto monster : map->GetMonstersVector())
	{
		if (monster->GetPosition() == nextMovePosition)
		{
			if (monster->IsAlive())
			{
				canMove = false;;
				return;
			}
		}
	}

	for (auto mapChangeController : map->GetActionMapChangeList())
	{
		if (mapChangeController->GetPosition() == nextMovePosition)
		{
			GSPlaying* gameStatePlaying = dynamic_cast<GSPlaying*>(Globals::game->GetGameState( "Play" ));
			if (gameStatePlaying)
			{
				gameStatePlaying->ChangeMap( mapChangeController->GetNextMapName() );
				SetPosition( mapChangeController->GetLandPosition() );
				flposition_x = static_cast<float>(m_position.x * Globals::tilesize);
				flposition_y = static_cast<float>(m_position.y * Globals::tilesize);

				Globals::camera->Center( this );

				return;
			}
		}
	}

	if( !map->TileExistsAtPosition( nextMovePosition.x, nextMovePosition.y ) )
	{
		canMove = false;
		return;
	}

	if (canMove)
	{
		SetPosition( nextMovePosition );
	}
}

int Player::GetWeaponDamage( void ) 
{
	switch( m_weaponType )
	{
	case CROWBAR:
		return CROWBAR_DAMAGE + GetSkills().m_battle;
		break;

	case PISTOL:
		return PISTOL_DAMAGE + GetSkills().m_battle;
		break;
	}

	return 0;
}

void Player::GiveItem( Item* item )
{
	if( item && HasSpaceInInventory() )
	{
		m_itemsVector.push_back( item );
		Globals::playerBelt->InsertItemToInventory( item );
		std::cout << "Przedmiotow w ekwipunku: " << m_itemsVector.size() << std::endl;
	}
}

bool Player::HasSpaceInInventory( void ) const
{
	return m_itemsVector.size() < INVENTORY_LIMIT ? true : false;
}


Skill& Player::GetSkillOfType( skillTypes::skillType_t skillType )
{
	for( std::vector<Skill>::iterator it = m_skillsVector.begin() ; it != m_skillsVector.end() ; ++it )
	{
		if( it->GetSkillType() == skillType )
		{
			return (*it);
		}
	}
}

void Player::SetTimeToNextAttack( float time )
{
	m_nextAttack = Globals::currentTime + time;
}

bool Player::IsCooldownReadyToAttack( void )
{
	return( Globals::currentTime >= m_nextAttack );
}

bool Player::IsEnemyInAttackDistance( int distance, Monster* m_attackedMonster )
{
	return( static_cast<int>( Tools::CalculateDistance( m_position.x, m_position.y, m_attackedMonster->GetPosition().x, m_attackedMonster->GetPosition().y ) ) <= distance );
}

void Player::AttackMonster( Monster* monster )
{ 
	switch( GetWeaponType() )
	{
	case CROWBAR:
		AttackMonsterWithMaleeWeapon( monster );
		break;

	case PISTOL:
		AttackMonsterWithDistanceWeapon( monster );
		break;
	}
}

void Player::AttackMonsterWithMaleeWeapon( Monster* monster )
{
	if( IsCooldownReadyToAttack() && IsEnemyInAttackDistance( MALEE_ATTACK_RANGE, monster ) )
	{
		monster->SetHealthPoints( monster->GetHealthPoints() - GetWeaponDamage() );
		SetTimeToNextAttack( ATTACK_DELAY_MALEE );
	}
}

void Player::AttackMonsterWithDistanceWeapon( Monster* monster )
{
	if( CanFireAtTheMonster( monster ) )
	{
		monster->SetHealthPoints( monster->GetHealthPoints() - GetWeaponDamage() );
		RemovePistolAmmunition();
		SetTimeToNextAttack( ATTACK_DELAY_GUN );
	}
}

bool Player::CanFireAtTheMonster( Monster* monster )
{
	return ( GetPistolAmmunition() && IsCooldownReadyToAttack() && IsEnemyInAttackDistance( PISTOL_ATTACK_RANGE, monster ) );
}

void Player::RemovePistolAmmunition( void )
{
	m_pistolAmmunition--;
}

void Player::GiveAmmunition( int count )
{
	m_pistolAmmunition += count;
}

void Player::LevelUpDamage( void )
{
	int experienceRequiredToLevelUp = GetSkills().m_battle * 2;
	if( GetExperiencePoints() >= experienceRequiredToLevelUp )
	{
		//SetBaseDamage( m_player->GetBaseDamage() + 1 );
		GetSkills().m_battle += 1;
		SetExperiencePoints( GetExperiencePoints() - experienceRequiredToLevelUp );
	}
}

void Player::CheckReductionInSpeedAtLowHealthPoints( void )
{
	m_healthPoints <= 10 ? m_velocity = BASE_VELOCITY : m_velocity = BASE_VELOCITY + 50.0f;
}

void Player::set_clips()
{
	//Clip the sprites
    clipsLeft[ 0 ].x = 0;
    clipsLeft[ 0 ].y = 100;
    clipsLeft[ 0 ].w = FOO_WIDTH;
    clipsLeft[ 0 ].h = FOO_HEIGHT;

    clipsLeft[ 1 ].x = FOO_WIDTH;
    clipsLeft[ 1 ].y = 100;
    clipsLeft[ 1 ].w = FOO_WIDTH;
    clipsLeft[ 1 ].h = FOO_HEIGHT;

    clipsLeft[ 2 ].x = FOO_WIDTH * 2;
    clipsLeft[ 2 ].y = 100;
    clipsLeft[ 2 ].w = FOO_WIDTH;
    clipsLeft[ 2 ].h = FOO_HEIGHT;

    clipsRight[ 0 ].x = 0;
    clipsRight[ 0 ].y = 200;
    clipsRight[ 0 ].w = FOO_WIDTH;
    clipsRight[ 0 ].h = FOO_HEIGHT;

    clipsRight[ 1 ].x = FOO_WIDTH;
    clipsRight[ 1 ].y = 200;
    clipsRight[ 1 ].w = FOO_WIDTH;
    clipsRight[ 1 ].h = FOO_HEIGHT;

    clipsRight[ 2 ].x = FOO_WIDTH * 2;
    clipsRight[ 2 ].y = 200;
    clipsRight[ 2 ].w = FOO_WIDTH;
    clipsRight[ 2 ].h = FOO_HEIGHT;

	clipsDown[ 0 ].x = 0;
    clipsDown[ 0 ].y = 0;
    clipsDown[ 0 ].w = FOO_WIDTH;
    clipsDown[ 0 ].h = FOO_HEIGHT;

    clipsDown[ 1 ].x = FOO_WIDTH;
    clipsDown[ 1 ].y = 0;
    clipsDown[ 1 ].w = FOO_WIDTH;
    clipsDown[ 1 ].h = FOO_HEIGHT;

    clipsDown[ 2 ].x = FOO_WIDTH * 2;
    clipsDown[ 2 ].y = 0;
    clipsDown[ 2 ].w = FOO_WIDTH;
    clipsDown[ 2 ].h = FOO_HEIGHT;

	clipsUp[ 0 ].x = 0;
    clipsUp[ 0 ].y = 300;
    clipsUp[ 0 ].w = FOO_WIDTH;
    clipsUp[ 0 ].h = FOO_HEIGHT;

    clipsUp[ 1 ].x = FOO_WIDTH;
    clipsUp[ 1 ].y = 300;
    clipsUp[ 1 ].w = FOO_WIDTH;
    clipsUp[ 1 ].h = FOO_HEIGHT;

    clipsUp[ 2 ].x = FOO_WIDTH * 2;
    clipsUp[ 2 ].y = 300;
    clipsUp[ 2 ].w = FOO_WIDTH;
    clipsUp[ 2 ].h = FOO_HEIGHT;
}