#include "Player.h"

#include "ActionMapChange.h"
#include "Box.h"
#include "Camera.h"
#include "FoodGenerator.h"
#include "Game.h"
#include "Globals.h"
#include "GSBattleFight.h"
#include "GSPlaying.h"
#include "Item.h"
#include "Map.h"
#include "Monster.h"
//#include "PlayerBelt.h"
#include "SpriteManager.h"
#include "StaticMapItem.h"
#include "Item.h"
#include "Tools.h"

#include <math.h>

Player::Player()
{
	m_name = "Gracz";

	m_position.x = Globals::currentMap->GetStartPosition().x;
	m_position.y = Globals::currentMap->GetStartPosition().y;
	flposition_x = static_cast<float>( m_position.x * Globals::tilesize );
	flposition_y = static_cast<float>( m_position.y * Globals::tilesize );

	m_velocity = BASE_VELOCITY;

	m_maxHealthPoints = 100;
	m_healthPoints = m_maxHealthPoints;

	m_maxActionPoints = 100;
	m_actionPoints = m_maxActionPoints;

	m_maxFood = 99;
	m_food = 5;
	m_toolbox = 0;
	m_junk = 0;
	m_experiencePoints = 0;

	m_selected = true;
	m_damage = 1;
	//Umiejetnosci itp.
	m_morphicHammer = false;
	m_miningHammerDurability = 3;
	m_miningAbility = false;

	m_attackedMonster = NULL;
	m_nextAttack = 0;
	m_attackSpeed = 1;

	m_weaponType = CROWBAR;
	m_pistolAmmunition = 4;

	m_sleeping = false;
	m_sleepingTime = 0;
	Globals::spriteManager->GetSprite( m_sleepingPlayerSprite, "player_sleeping" );

	Globals::spriteManager->GetSprite( m_sprite, "playertest" );
	Globals::spriteManager->GetSprite( m_spriteSelected, "championselection" );
	Globals::currentPlayer = this;
}

void Player::Think( void )
{
	if( !IsMoving() )
	{
		AttackingMonster();
	}

	if( m_sleeping )
	{
		if( Globals::currentTime > m_sleepingTime + SLEEPING_TIME )
		{
			m_sleeping = false;
			std::cout << "Koniec spania." << std::endl; //console info
		}
	}

	if( m_actionPoints <= 0 )
	{
		m_velocity = BASE_VELOCITY / 2;
	}

	else
	{
		m_velocity = BASE_VELOCITY;
	}
}

void Player::Draw( void )
{
	if( m_sleeping )
	{
		m_sleepingPlayerSprite.Draw( Globals::screen, m_position );
	}

	GameObject::Draw();
}

void Player::Move( direction_t direction )
{
	if( IsMoving() || m_sleeping )
	{
		return;
	}

	bool canMove = true;
	Position newPosition = GetPosition();

	std::vector<StaticMapItem*>&	staticMapItems = Globals::currentMap->GetStaticMapItemVector();
	std::vector<Monster*>&			monsters = Globals::currentMap->GetMonstersVector();
	std::vector<Item*>&			Items = Globals::currentMap->GetItemsVector();
	std::vector<FoodGenerator*>&	foodGenerators = Globals::currentMap->GetFoodGeneratorsVector();

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

	for( std::vector<StaticMapItem*>::const_iterator cit = staticMapItems.begin() ; cit != staticMapItems.end() ; ++cit )
	{
		if( ( *cit )->GetPosition().x == newPosition.x && ( *cit)->GetPosition().y == newPosition.y )
		{
			if( (*cit)->IsWalkable() == false )
			{
				canMove = false;
				return;		// Jak jest kolizja to konczymy dzialanie funkcji bo nie ma sensu dalej kombinowac.
			}

			else
			{
				canMove = true;
			}		
		}
	}

	for( std::vector<Monster*>::const_iterator cit = monsters.begin() ; cit != monsters.end() ; ++cit )
	{
		if( ( *cit )->GetPosition().x == newPosition.x && ( *cit )->GetPosition().y == newPosition.y )
		{
			if( ( *cit )->IsAlive() )
			{
				canMove = false;
				//AttackMonster( (*cit) );
				return;
			}

			else
				canMove = true;
		}
	}

	/*for( std::vector<Item*>::const_iterator cit = Items.begin() ; cit != Items.end() ; ++cit )
	{
		Item* Item = ( *cit );
		if( Item->GetPosition().x == newPosition.x && Item->GetPosition().y == newPosition.y )
		{
			GSPlaying* gsPlaying = dynamic_cast<GSPlaying*>( Globals::game->GetGameState( "Play" ) );
			if( gsPlaying )
			{
				gsPlaying->PlayerPickItem( Item );
			}

			return;
		}
	}*/

	std::list<ActionMapChange*>& actionMapChangeList = Globals::currentMap->GetActionMapChangeList();
	std::list<ActionMapChange*>::const_iterator cit;
	for( cit = actionMapChangeList.begin() ; cit != actionMapChangeList.end() ; ++cit )
	{
		if( ( *cit )->GetPosition().x == newPosition.x && ( *cit )->GetPosition().y == newPosition.y )
		{
			GSPlaying* gsPlaying = dynamic_cast<GSPlaying*>( Globals::game->GetGameState( "Play" ) );
			if( gsPlaying )
			{
				gsPlaying->ChangeMap( ( *cit )->GetNextMapName() );
				SetPosition( ( *cit )->GetLandPosition() );

				Globals::camera->Center( this );

				return;
			}

		}
	}

	Map* map = Globals::currentMap;
	if( !map->TileExistsAtPosition( newPosition.x, newPosition.y ) )
	{
		canMove = false;
	}

	if( canMove /*&& m_actionPoints > 0*/ && m_selected == true )
	{
		Globals::camera->FollowPlayer( true );
		//Globals::camera->Center( this );

		if( m_actionPoints > 0 )
		{
			m_actionPoints--;
		}

		else
		{
			m_actionPoints = 0;
		}

		SetPosition( newPosition );
		m_nextAttack = Globals::currentTime + ATTACK_DELAY; //W czasie ruchu nie mozemy atakowac.
	}
}

void Player::PickBox( void )
{
	std::vector<Box*>& boxes = Globals::currentMap->GetBoxes();
	std::vector<Box*>::const_iterator cit;

	for( cit = boxes.begin() ; cit != boxes.end() ; ++cit )
	{
		if( ( *cit )->GetPosition().x == GetPosition().x && ( *cit )->GetPosition().y == GetPosition().y )
		{

		}
	}
}

void Player::AttackMonster( Monster* monster )
{
	if( m_actionPoints > 0 )
	{
		monster->SetHealthPoints( monster->GetHealthPoints() - m_damage ); //atakujemy stwora
		m_healthPoints -= monster->GetBaseDamage();	//kontra
		
		if( m_morphicHammer == true )
		{
			m_junk++;
		}
	}

	std::cout << "Atak na potworka, tera jego HP: " << monster->GetHealthPoints() << std::endl;
	std::cout << "Moje HP: " << m_healthPoints << std::endl;
}

void Player::GiveFood( int food )
{
	int newFood = GetFood() + food;

	if( newFood > GetMaxFood() )
	{
		SetFood( GetMaxFood() );
	}

	else if( newFood < 0 )
	{
		SetFood( 0 );
	}

	else
	{
		SetFood( newFood );
	}
}

void Player::SetAttackedMonster( Monster* monster )
{
	if( monster && monster->IsAlive() )
	{
		m_attackedMonster = monster;
		m_attackedMonster->SetAttackedByPlayer( true );
	}
}

void Player::AttackingMonster( void )
{
	if( m_attackedMonster && m_attackedMonster->IsAlive() )
	{
		if( m_weaponType == CROWBAR )
		{
			if( static_cast<int>( Tools::CalculateDistance( m_position.x, m_position.y, m_attackedMonster->GetPosition().x, m_attackedMonster->GetPosition().y ) ) == 1 )
			{
				if( Globals::currentTime >= m_nextAttack )
				{
					std::cout << "Atakujemy Spidera[" << m_attackedMonster->GetHealthPoints() << " HP] zadajac mu " << GetBaseDamage() + GetWeaponDamage() << " punktow obrazen!" << std::endl;
					if( m_actionPoints <= 0 )
					{
						m_attackedMonster->SetHealthPoints( m_attackedMonster->GetHealthPoints() - ( CROWBAR_DAMAGE / 2 )- m_damage ); //Przy zmeczeniu zadajemy polowe mnie obrazej przy broni bialej.
					}

					else
					{
						m_attackedMonster->SetHealthPoints( m_attackedMonster->GetHealthPoints() - CROWBAR_DAMAGE - m_damage );
					}

					if( m_attackedMonster->GetHealthPoints() <= 0 )
					{
						m_attackedMonster = NULL;
						m_experiencePoints++;

						if( m_morphicHammer )
						{
							m_junk++;
						}
					}

					m_nextAttack = Globals::currentTime + ATTACK_DELAY;
				}
			}
		}

		if( m_weaponType == PISTOL && m_pistolAmmunition > 0 )
		{
			if( static_cast<int>( Tools::CalculateDistance( m_position.x, m_position.y, m_attackedMonster->GetPosition().x, m_attackedMonster->GetPosition().y ) ) <= PISTOL_ATTACK_RANGE )
			{
				if( Globals::currentTime >= m_nextAttack )
				{
					std::cout << "Atakujemy Spidera[" << m_attackedMonster->GetHealthPoints() << " HP] zadajac mu " << GetBaseDamage() + GetWeaponDamage() << " punktow obrazen!" << std::endl;

					m_pistolAmmunition--;
					if( m_pistolAmmunition < 0 )
					{
						m_pistolAmmunition = 0;
					}

					m_attackedMonster->SetHealthPoints( m_attackedMonster->GetHealthPoints() - PISTOL_DAMAGE - m_damage );

					if( m_attackedMonster->GetHealthPoints() <= 0 )
					{
						m_attackedMonster = NULL;
						m_experiencePoints++;

						if( m_morphicHammer )
						{
							m_junk++;
						}
					}

					m_nextAttack = Globals::currentTime + ATTACK_DELAY;
				}
			}
		}
	}
}

void Player::CheckMonsterAttackDistance( void )
{
	if( m_attackedMonster )
	{
		if( static_cast<int>( Tools::CalculateDistance( m_position.x, m_position.y, m_attackedMonster->GetPosition().x, m_attackedMonster->GetPosition().y ) ) > MAX_ATTACK_DISTANCE )
		{
			m_attackedMonster->SetAttackedByPlayer( false );
			m_attackedMonster = NULL;
		
			//console information
			std::cout << "Monster uciekl z zasiegu MAX_ATTACK_DISTANCE" << std::endl;
		}
	}
}

void Player::GoSleep( void )
{
	if( !m_sleeping )
	{
		m_sleepingTime = Globals::currentTime;
		m_sleeping = true;
	}
}

int Player::GetWeaponDamage( void ) const
{
	switch( m_weaponType )
	{
	case CROWBAR:
		return CROWBAR_DAMAGE;
		break;

	case PISTOL:
		return PISTOL_DAMAGE;
		break;
	}

	return 0;
}

void Player::GiveItem( Item* item )
{
	if( item && HasSpaceInInventory() )
	{
		m_itemsVector.push_back( item );
		std::cout << "Przedmiotow w ekwipunku: " << m_itemsVector.size() << std::endl;
	}
}

bool Player::HasSpaceInInventory( void ) const
{
	if( m_itemsVector.size() < INVENTORY_LIMIT )
	{
		return true;
	}

	else
	{
		std::cout << "Nie ma miejsca w ekwipunku." << std::endl;
		return false;
	}
}