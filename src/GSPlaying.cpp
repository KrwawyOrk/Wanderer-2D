#include "GSPlaying.h"

#include "Camera.h"
#include "Container.h"
#include "Factory.h"
#include "Game.h"
#include "Globals.h"
#include "GSMapEditor.h"
#include "ItemSlot.h"
#include "MapEntitiesLoader.h";
#include "SpriteManager.h"
#include "StaticMapItem.h"
#include "Tile.h"
#include "Tools.h"
#include "Item.h"

#include <iostream>
#include <sstream>
#include <math.h>

#include "Sprite.h"

GSPlaying::GSPlaying( void )
{
	for( int i = 0 ; i < 323 ; i++ )
	{
		m_keysHeld[i] = false;
	}

	m_mapList.push_back( new Map( "lasek" ) );
	m_mapList.push_back( new Map( "lasek2" ) );

	MapEntitiesLoader mapEntitiesLoader;
	mapEntitiesLoader.LoadLaboratoryEntities( GetMapByName( "lasek" ) );

	ChangeMap( "lasek" );

	m_playingState = GAMEPLAY;

	m_player = new Player;
	Globals::player = m_player;
	m_playerBelt.SetPlayer( Globals::player );

	Globals::camera->Center( Globals::player );
	Globals::camera->FollowPlayer( true );

	m_fishing = false;
	m_day = 1;

	m_exitLocationMessage = new BitmapFont( FontStyle::FONT_WHITE_SMALL_OLD );

	Globals::playerBelt = &m_playerBelt;

	m_openedContainer = NULL;
	m_npc = NULL;
}

GSPlaying::~GSPlaying( void )
{

}

void GSPlaying::InputEvents( void )
{
	if( m_openedContainer )
	{
		m_openedContainer->InputEvents();
	}

	if( Globals::event.type == SDL_MOUSEBUTTONDOWN )
	{
		if( Globals::event.button.button == SDL_BUTTON_LEFT )
		{
			Position position; //pozycja wcisniecia przycisku myszki
			position.x = ( Globals::event.button.x + Globals::camera->GetCameraX() ) / Globals::tilesize;
			position.y = ( Globals::event.button.y + Globals::camera->GetCameraY() ) / Globals::tilesize;


			std::vector<Item*> &items = Globals::currentMap->GetItemsVector();
			std::vector<Item*>::iterator it;
			for( it = items.begin() ; it != items.end() ; )
			{
				Item* item = ( *it );
				int distanceToPick = static_cast<int>(Tools::CalculateDistance( m_player->GetPosition().x, m_player->GetPosition().y, item->GetPosition().x, item->GetPosition().y ));

				if( m_player->HasSpaceInInventory() && item->GetPosition().x == position.x && item->GetPosition().y == position.y && distanceToPick <= 1 )
				{
					it = items.erase( it );
					m_player->GiveItem( item );
				}

				else
				{
					++it;
				}
			}


			std::cout << "Pozycja myszka-button-left: x: " << Globals::event.button.x << " y: " << Globals::event.button.y << std::endl;
			std::cout << "Pozycja po konwersji: x: " << position.x  << " y: " << position.y  << std::endl;
		}

		if( Globals::event.button.button == SDL_BUTTON_RIGHT )
		{
			Position position; //pozycja wcisniecia przycisku myszki
			position.x = ( Globals::event.button.x + Globals::camera->GetCameraX() ) / Globals::tilesize;
			position.y = ( Globals::event.button.y + Globals::camera->GetCameraY() ) / Globals::tilesize;

	
			std::vector<Monster*>&monsters = Globals::currentMap->GetMonstersVector();

			for( std::vector<Monster*>::const_iterator cit = monsters.begin() ; cit != monsters.end() ; ++cit )
			{
				Monster* monster = ( *cit );
				monster->SetAttackedByPlayer( false );

				if(monster->GetPosition().x == position.x && monster->GetPosition().y == position.y )
				{
					if(monster->IsAlive() )
					{
						m_player->AttackMonster( monster );

						if (m_openedContainer)
						{
							m_openedContainer->setWindowVisible( false );
						}
					}
				}
			}

			std::vector<StaticMapItem*> &staticMapItems = Globals::currentMap->GetStaticMapItemVector();

			for( std::vector<StaticMapItem*>::const_iterator cit = staticMapItems.begin() ; cit != staticMapItems.end() ; ++cit )
			{
				StaticMapItem* staticMapItem = ( *cit );
				if( staticMapItem->GetPosition().x == position.x && staticMapItem->GetPosition().y == position.y )
				{
					int distance = static_cast<int>( Tools::CalculateDistance( staticMapItem->GetPosition().x, staticMapItem->GetPosition().y, m_player->GetPosition().x, m_player->GetPosition().y ) );

					if( staticMapItem->HasContainer() && distance <= 1 )
					{
						if( staticMapItem->GetContainer()->IsLocked() )
						{
							std::cout << "Potrzebujesz wytrychu." << std::endl;
						}

						else
						{
							staticMapItem->GetContainer()->setWindowVisible( true );
							m_openedContainer = staticMapItem->GetContainer();
						}
					}

					std::cout << "Wcisnales PPM na StaticMapItem." << std::endl;
				}
			}
		}
	}


	if( Globals::event.type == SDL_MOUSEMOTION )
	{
		MoveCameraOnMouseMotion();
	}

	if( Globals::event.type == SDL_KEYDOWN )
	{
		m_keysHeld[Globals::event.key.keysym.sym] = true;

		switch( Globals::event.key.keysym.sym )
		{
		case SDLK_q:
		case SDLK_ESCAPE:
			Globals::game->Drop();
			break;

		case SDLK_KP4:
			Globals::camera->Move( LEFT );
			break;

		case SDLK_KP6:
			Globals::camera->Move( RIGHT );
			break;

		case SDLK_KP8:
			Globals::camera->Move( UP );
			break;

		case SDLK_KP2:
			Globals::camera->Move( DOWN );
			break;

		case SDLK_b:
			
			break;

		case SDLK_c:
			Globals::camera->SetPosition( 0 , 0 );
			break;

		case SDLK_i:
			PrintInformationsInConsole();
			break;

		case SDLK_u:
			m_player->LevelUpDamage();
			break;

		case SDLK_y:
			Globals::showCursor = !Globals::showCursor;
			break;

		case SDLK_SPACE:
		case SDLK_TAB:
			Globals::camera->Center( m_player );
			break;

		case SDLK_p:
			{
				bool b = m_player->GetSkillOfType( skillTypes::INCREASED_VISIBILITY ).SkillIsLearned();
				m_player->GetSkillOfType( skillTypes::INCREASED_VISIBILITY ).SetLearnedSkill( !b );
			}
			break;

		case SDLK_1:
			m_player->SetWeapon( CROWBAR );
			break;

		case SDLK_2:
			m_player->SetWeapon( PISTOL );
			break;

		case SDLK_F1:
			Globals::camera->FollowPlayer( !Globals::camera->GetCameraFollowPlayer() );
			break;

		case SDLK_F9:
			OpenMapEditorToEditCurrentMap();
			break;

		case SDLK_F10:
			OpenMapEditorNewMapWindow();
			break;

		case SDLK_RETURN:
			PlayerLeaveMap();
			break;
		}
	}

	else if( Globals::event.type == SDL_KEYUP )
	{
		m_keysHeld[Globals::event.key.keysym.sym] = false;
	}

	m_playerBelt.InputEvents();
}

void GSPlaying::Think( void )
{
	//Globals::camera->Think();
	m_map->Think();
	m_player->Think();
	m_playerBelt.Think();

	if (m_player->IsMoving())
	{
		m_npc = NULL;
	}

	CheckIfContainerIsOpened();
	ThinkMovementInputButtonsHeld();
}

void GSPlaying::ThinkMovementInputButtonsHeld( void )
{
	if (m_keysHeld[SDLK_UP] || m_keysHeld[SDLK_w])
	{
		m_player->Move( NORTH );
	}

	else if (m_keysHeld[SDLK_DOWN] || m_keysHeld[SDLK_s])
	{
		m_player->Move( SOUTH );
	}

	else if (m_keysHeld[SDLK_RIGHT] || m_keysHeld[SDLK_d])
	{
		m_player->Move( EAST );
	}

	else if (m_keysHeld[SDLK_LEFT] || m_keysHeld[SDLK_a])
	{
		m_player->Move( WEST );
	}
}

void GSPlaying::Update( float deltaTime )
{
	m_map->Update( deltaTime );
	m_player->Update( deltaTime );
	Globals::camera->Update( deltaTime );
}

void GSPlaying::Draw( void )
{
	m_map->Draw();
	m_player->Draw();
	m_playerBelt.Draw();
	DrawExitLocationMessage();
	DrawOpenedContainer();
}

Map* GSPlaying::GetMapByName( std::string mapName )
{
	std::list<Map*>::iterator it;
	for( it = m_mapList.begin() ; it != m_mapList.end() ; ++it )
	{
		if( ( *it )->GetMapName() == mapName )
			return *it;
	}

	return NULL;
}

bool GSPlaying::MapExists( std::string mapName ) const
{
	std::list<Map*>::const_iterator cit;
	for( cit = m_mapList.begin() ; cit != m_mapList.end() ; ++cit )
	{
		if( ( *cit )->GetMapName() == mapName )
			return true;
	}

	return false;
}

void GSPlaying::ChangeMap( std::string mapName )
{
	std::list<Map*>::const_iterator cit;
	for( cit = m_mapList.begin() ; cit != m_mapList.end() ; ++cit )
	{
		if( ( *cit )->GetMapName() == mapName )
		{
			m_map = *cit;
			Globals::currentMap = m_map;

			std::cout << "// Zmiana mapy na " << ( *cit )->GetMapName() << std::endl;
			return;
		}
	}
}

void GSPlaying::PlaceProtectionZone( Position& position )
{
	for( std::vector<Tile*>::iterator it = m_map->GetTilesVector().begin() ; it != m_map->GetTilesVector().end() ; ++it )
	{
		if( position.x == ( *it )->GetPosition().x && position.y == ( *it )->GetPosition().y )
		{
			( *it )->SetProtectionZone( true );
		}
	}
}

void GSPlaying::PlayerLeaveMap( void )
{
	Map* map = Globals::currentMap;
	if( m_player->GetPosition().x == map->GetExitPosition().x && m_player->GetPosition().y == map->GetExitPosition().y )
	{
		std::cout << "Opuszczamy mape" << std::endl;
		Globals::game->SetGameState( "World map" );


		// ---------------------- -----------BUG FIX ---------------------------------
		//fix buga, czasem po powrocie z lokalizacji nasza postac sama sie poruszala...
		//ktos cos? wazne, ze po dodaniu ponizszego kawalka kodu wszystko dziala :)
		//wyglada na to, ze input klawiatury dostal zawieszki :o
		for( int i = 0 ; i < 323 ; i++ )
		{
			m_keysHeld[i] = false;
		}
		//koniec fix buga
	}
}

void GSPlaying::DrawExitLocationMessage( void )
{
	if( m_player->GetPosition().x == m_map->GetExitPosition().x && m_player->GetPosition().y == m_map->GetExitPosition().y )
	{
		m_exitLocationMessage->show_text( 1280 / 2, 720 / 2, "Press ENTER to exit.", Globals::screen );
	}
}

void GSPlaying::DrawOpenedContainer( void )
{
	if( m_openedContainer )
	{
		m_openedContainer->Draw();
	}
}

void GSPlaying::CheckIfContainerIsOpened( void )
{
	if( m_openedContainer )
	{
		m_openedContainer->Think();
	}

	if( m_openedContainer && m_player->IsMoving() )
	{
		m_openedContainer = NULL;
	}
}

void GSPlaying::OpenMapEditorToEditCurrentMap( void )
{
	if (Globals::developer == true)
	{
		GSMapEditor* mapEditor = dynamic_cast<GSMapEditor*>(Globals::game->GetGameState( "Map editor" ));
		if (mapEditor)
		{
			mapEditor->GiveMap( m_map );
			mapEditor->SetBrushPosition( GetPlayer()->GetPosition() );
			mapEditor->SetMapEditorState( MAPPING );

			Globals::camera->FollowPlayer( false );
			Globals::game->SetGameState( "Map editor" );
		}
	}
}

void GSPlaying::OpenMapEditorNewMapWindow( void )
{
	if (Globals::developer == true)
	{
		GSMapEditor* mapEditor = dynamic_cast<GSMapEditor*>(Globals::game->GetGameState( "Map editor" ));
		if (mapEditor)
		{
			mapEditor->SetMapEditorState( MENU );
			Globals::game->SetGameState( "Map editor" );
		}
	}
}

void GSPlaying::PrintInformationsInConsole( void )
{
	std::cout << m_player->GetPosition().x << "\t" << m_player->GetPosition().y << std::endl;
	std::cout << "Pozycja startowa mapy: " << Globals::currentMap->GetStartPosition().x << "\t" << Globals::currentMap->GetStartPosition().y << std::endl;
	std::cout << "Pozycja wyjsciowa mapy: " << Globals::currentMap->GetExitPosition().x << "\t" << Globals::currentMap->GetExitPosition().y << std::endl;
	std::cout << "Punkty zycia: " << m_player->GetHealthPoints() << "/" << m_player->GetMaxHealthPoints() << std::endl;
	std::cout << "Pozycja kamery: x: " << Globals::camera->GetCameraX() << " y: " << Globals::camera->GetCameraY() << std::endl;
}

void GSPlaying::MoveCameraOnMouseMotion( void )
{
	int mouse_x = Globals::event.motion.x;
	int mouse_y = Globals::event.motion.y;

	if (mouse_y < 2)
	{
		Globals::camera->velocity_y = -Globals::camera->cameraVelocity;
	}

	else if (mouse_y > Globals::resolution_y - 2)
	{
		Globals::camera->velocity_y = Globals::camera->cameraVelocity;
	}

	else if (mouse_x < 2)
	{
		Globals::camera->velocity_x = -Globals::camera->cameraVelocity;
	}

	else if (mouse_x > Globals::resolution_x - 2)
	{
		Globals::camera->velocity_x = Globals::camera->cameraVelocity;
	}

	else
	{
		Globals::camera->velocity_x = 0;
		Globals::camera->velocity_y = 0;

	}
}