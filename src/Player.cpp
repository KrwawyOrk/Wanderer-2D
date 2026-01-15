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
#include "Particle.h"
#include "SpriteManager.h"
#include "StaticMapItem.h"
#include "Item.h"
#include "TextBox.h"
#include "Tools.h"

#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "nlohmann/json.hpp"

//using enum ParticleType;
using namespace rapidxml;


Player::Player()
{
	//LoadPlayerDataFromXML( "pliki/player.xml" );
	LoadPlayerDataFromJSON();
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

	SetClips();
	//Initialize movement variables
    offSet = 0;
 
    //Initialize animation variables
    frame = 0;
    status = FOO_RIGHT;

	m_flashlightBattery = 1.0f;

	Globals::spriteManager->GetSprite( m_animationSprite, "foo" );
}

void Player::LoadPlayerDataFromJSON( void )
{
	std::ifstream file( "pliki/player.json" );
	nlohmann::json player;
	file >> player;

	m_healthPoints = player["health"];
	m_maxHealthPoints = player["maxHealth"];
	m_experiencePoints = player["experiencePoints"];
	m_monstersKilled = player["monstersKilled"];
	m_pistolAmmunition = player["pistolAmmunition"];
	m_damage = player["damage"];
	m_position.x = player["position_x"];
	m_position.y = player["position_y"];
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
	if( IsMovingLeft() )
    {
        status = FOO_LEFT;
		frame = GetFrame( GetAnimationSpeedBasedOnPlayerVelocity() );
    }

    else if( IsMovingRight() )
    {
        status = FOO_RIGHT;
		frame = GetFrame( GetAnimationSpeedBasedOnPlayerVelocity() );
    }
	
	else if( IsMovingUp() )
	{
		status = FOO_UP;
		frame = GetFrame( GetAnimationSpeedBasedOnPlayerVelocity() );
	}

	else if( IsMovingDown() )
	{
		status = FOO_DOWN;
		frame = GetFrame( GetAnimationSpeedBasedOnPlayerVelocity() );
	}

    if( !IsMoving() )
    {
        frame = 1;
    }

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
		CheckPlayerActionEventsPosition( GetPosition().x, GetPosition().y, Globals::currentMap->GetMapActions() );
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
		Globals::camera->StartShake( 6.0f, 0.30f );
		SetTimeToNextAttack( ATTACK_DELAY_MALEE );

		float hitX = (monster->GetPosition().x * Globals::tilesize) + Globals::tilesize / 2.0f;
		float hitY = (monster->GetPosition().y * Globals::tilesize) + Globals::tilesize / 2.0f;

		GSPlaying* gsPlaying = dynamic_cast<GSPlaying*>(Globals::game->GetGameState( "Play" ));
		if (gsPlaying)
		{
			gsPlaying->EmitParticles( hitX, hitY, ParticleType::GREEN_BLOOD, 25 );
		}
	}
}

void Player::AttackMonsterWithDistanceWeapon( Monster* monster )
{
	if( CanFireAtTheMonster( monster ) )
	{
		monster->SetHealthPoints( monster->GetHealthPoints() - GetWeaponDamage() );
		RemovePistolAmmunition();
		
		float hitX = (monster->GetPosition().x * Globals::tilesize) + Globals::tilesize / 2.0f;
		float hitY = (monster->GetPosition().y * Globals::tilesize) + Globals::tilesize / 2.0f;

		GSPlaying* gsPlaying = dynamic_cast<GSPlaying*>(Globals::game->GetGameState( "Play" ));
		if (gsPlaying)
		{
			gsPlaying->EmitParticles( hitX, hitY, ParticleType::GREEN_BLOOD, 25 );
		}

		Globals::camera->StartShake( 4.5f, 0.18f );
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

	m_healthPoints < m_maxHealthPoints * 0.3 ? m_velocity = BASE_VELOCITY / 2 : m_velocity = BASE_VELOCITY;
}

int Player::GetFrame( int animationSpeed )
{
	return int( ((SDL_GetTicks() / animationSpeed) % 3) );
}

int Player::GetAnimationSpeedBasedOnPlayerVelocity()
{
	return std::max( 50, 200 - (int)m_velocity );
}

void Player::PrintLevelAdvanceInformations( void )
{
	m_playerAdvanceLevel.PrintLevelAdvanceInformations( this );
}

void Player::GiveExperiencePoints( int points )
{
	int previousLevel = m_playerAdvanceLevel.CheckForLevelAdvance( this );

	m_experiencePoints += points;
	int currentLevel = m_playerAdvanceLevel.CheckForLevelAdvance( this );

	if (currentLevel > previousLevel)
	{
		std::cout << "Level advanced!!" << std::endl;
		Globals::messageLog->addLine( "Congratulations! You have advanced to next level!" );
	}
}

void Player::CheckPlayerActionEventsPosition( int player_x, int player_y, std::vector<json>& actions )
{
	for( auto& action : actions )
	{
		if( action["action_type"] == "step_on_tile" )
		{
			int tile_x = action["x"];
			int tile_y = action["y"];

			if( player_x == tile_x && player_y == tile_y )
			{
				std::string event_type = action["trigger_event"];

				if( event_type == "message_log" && !action["visited"] )
				{
					Globals::messageLog->addLine( action["text"] );
					action["visited"] = true;
				}

				else if( event_type == "remove_static_item" && !action["visited"] )
				{
					Globals::currentMap->RemoveStaticMapItem( action["remove_x"], action["remove_y"] );
					action["visited"] = true;
				}
			}
		}
	}
}

void Player::SetClips()
{
	for (int i = 0; i < 3; ++i)
	{
		Sint16 x = static_cast<Sint16>(FOO_WIDTH * i);

		clipsDown[i] = { x, 0,   FOO_WIDTH, FOO_HEIGHT };
		clipsLeft[i] = { x, 100, FOO_WIDTH, FOO_HEIGHT };
		clipsRight[i] = { x, 200, FOO_WIDTH, FOO_HEIGHT };
		clipsUp[i] = { x, 300, FOO_WIDTH, FOO_HEIGHT };
	}
}

void Player::SetFlashlightBattery( float level )
{
	m_flashlightBattery = level;
	if (m_flashlightBattery < 0.0f) m_flashlightBattery = 0.0f;
	if (m_flashlightBattery > 1.0f) m_flashlightBattery = 1.0f;
}

void Player::RechargeFlashlight( float amount )
{
	SetFlashlightBattery( m_flashlightBattery + amount );
}

void Player::DrainFlashlight( float amount )
{
	SetFlashlightBattery( m_flashlightBattery - amount );
}

void Player::GenerateBloodParticlesOnBeingHit( void )
{
	float hitX = ( GetPosition().x * Globals::tilesize ) + Globals::tilesize / 2.0f;
	float hitY = ( GetPosition().y * Globals::tilesize ) + Globals::tilesize / 2.0f;

	GSPlaying* gsPlaying = dynamic_cast<GSPlaying*>( Globals::game->GetGameState( "Play" ) );
	if( gsPlaying )
	{
		gsPlaying->EmitParticles( hitX, hitY, ParticleType::BLOOD, 25 );
	}
}