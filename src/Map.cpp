#include "Map.h"

#include "Camera.h"
#include "Factory.h"
#include "Game.h"
#include "Globals.h"
#include "SpriteManager.h"
#include "Tools.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

Map::Map( std::string mapName )
{
	m_mapName = mapName;
	Globals::spriteManager->GetSprite( m_questionMark, "questionmark" );
	Globals::spriteManager->GetSprite( m_exitSprite, "exittile" );

	LoadTiles();
	LoadStaticMapItems();
	LoadMonsters();
	LoadActionMapChange();
	LoadStartExitPositions();
}

Map::~Map()
{
	std::cout << "Niszcze obiekt klasy Map." << std::endl;

	for( std::vector<Tile*>::const_iterator cit = m_tiles.begin() ; cit != m_tiles.end() ; ++cit )
	{
		delete ( *cit );
	}

	for( std::vector<StaticMapItem*>::const_iterator cit = m_staticItems.begin() ; cit != m_staticItems.end() ; ++cit )
	{
		delete ( *cit );
	}

	for( std::vector<Monster*>::const_iterator cit = m_monsters.begin() ; cit != m_monsters.end() ; ++cit )
	{
		delete ( *cit );
	}

	for( std::vector<Item*>::const_iterator cit = m_Items.begin() ; cit != m_Items.end() ; ++cit )
	{
		delete ( *cit );
	}
}

void Map::InputEvents( void )
{

}

void Map::Think( void )
{
	std::vector<Monster*>::iterator cit;
	for( cit = m_monsters.begin() ; cit != m_monsters.end() ; )
	{
		( *cit )->Think();

		if( (*cit)->GetHealthPoints() <= 0 )
		{
			( *cit )->SetDeathTime( Globals::currentTime );
			( *cit )->CreateCorpseWithLootItems();

			MonsterRespawnInfo* respawn = new MonsterRespawnInfo;
			respawn->m_monsterName = ( *cit )->GetName();
			respawn->m_respawnPosition.x = ( *cit )->GetRespawnPosition().x;
			respawn->m_respawnPosition.y = ( *cit )->GetRespawnPosition().y;
			respawn->m_respawnTime = ( *cit )->GetRespawnTime();
			respawn->m_deathTime = ( *cit )->GetDeathTime();

			m_monstersWaitingForRespawn.push_back( respawn );

			delete ( *cit );
			cit = m_monsters.erase( cit );
		}

		else
			++cit;
	}

	std::vector<MonsterRespawnInfo*>::iterator it;
	for( it = m_monstersWaitingForRespawn.begin() ; it != m_monstersWaitingForRespawn.end() ; )
	{
		if( Globals::currentTime >= ( *it )->m_deathTime + ( *it )->m_respawnTime )
		{
			m_monsters.push_back( Globals::factory->CreateMonsterRespawn( ( *it )->m_monsterName, Position( ( *it )->m_respawnPosition.x, ( *it )->m_respawnPosition.y ), ( *it )->m_respawnTime ) );

			delete ( *it );
			it = m_monstersWaitingForRespawn.erase( it );
		}


		else
			++it;
	}

	for( std::vector<Tile*>::iterator it = m_tiles.begin() ; it != m_tiles.end() ; ++it )
	{
		Tile* tile = ( *it );
		if( Globals::camera->InCameraView( tile ) )
		{
			tile->Think();
		}
	}
}

void Map::Update( float deltaTime )
{
	for( std::vector<Monster*>::const_iterator cit = m_monsters.begin() ; cit != m_monsters.end() ; ++cit )
	{
		( *cit )->Update( deltaTime );
	}
}

void Map::Draw( void )
{
	for( std::vector<Tile*>::iterator it = m_tiles.begin() ; it != m_tiles.end() ; ++it )
	{
		Tile* tile = ( *it );
		if( Globals::camera->InCameraView( tile ) && Globals::game->GetGameStateEnum() == PLAYING )
		{
			/*if( tile->GetFogOfWar().IsExplored() )
			{
			tile->Draw();
			}*/

			Player* player = Globals::player;
			int distance = static_cast<int>( Tools::CalculateDistance( tile->GetPosition().x, tile->GetPosition().y, player->GetPosition().x, player->GetPosition().y ) );

			/*if( distance < DISTANCE_FROM_PLAYER_TO_RENDER )
			{
				tile->Draw();
			}*/

			if( player->GetSkillOfType( skillTypes::INCREASED_VISIBILITY ).SkillIsLearned() )
				{
					if( distance < DISTANCE_FROM_PLAYER_TO_RENDER + 3 ) 
					{
						tile->Draw();
					}
				}

				else
				{
					if( distance < DISTANCE_FROM_PLAYER_TO_RENDER ) 
					{
						tile->Draw();
					}
				}

		}

		else if( Globals::camera->InCameraView( tile ) && Globals::game->GetGameStateEnum() == MAPEDITOR )
		{
			tile->Draw();
		}
	}

	m_exitSprite.Draw( Globals::screen, m_exitPosition );

	for( std::vector<StaticMapItem*>::iterator it = m_staticItems.begin() ; it != m_staticItems.end() ; ++it )
	{
		StaticMapItem* staticMapItem = ( *it );
		if( Globals::camera->InCameraView( staticMapItem ) )
		{
			if( Globals::game->GetGameStateEnum() == MAPEDITOR )
			{
				staticMapItem->Draw();
			}

			else if( Globals::game->GetGameStateEnum() == PLAYING )
			{
				Player* player = Globals::player;
				int distance = static_cast<int>( Tools::CalculateDistance( staticMapItem->GetPosition().x, staticMapItem->GetPosition().y, player->GetPosition().x, player->GetPosition().y ) );

				/*if( distance < DISTANCE_FROM_PLAYER_TO_RENDER )
				{
				staticMapItem->Draw();
				}*/

				if( player->GetSkillOfType( skillTypes::INCREASED_VISIBILITY ).SkillIsLearned() )
				{
					if( distance < DISTANCE_FROM_PLAYER_TO_RENDER + 3 ) 
					{
						staticMapItem->Draw();
					}
				}

				else
				{
					if( distance < DISTANCE_FROM_PLAYER_TO_RENDER ) 
					{
						staticMapItem->Draw();
					}
				}

				/*for( std::vector<Tile*>::iterator it = m_tiles.begin() ; it != m_tiles.end() ; ++it )
				{
				Tile* tile = ( *it );

				if( tile->GetPosition().x == staticMapItem->GetPosition().x && tile->GetPosition().y == staticMapItem->GetPosition().y && tile->GetFogOfWar().IsExplored() )
				{
				staticMapItem->Draw();
				}
				}*/
			}
		}
	}

	for( std::vector<Item*>::iterator it = m_Items.begin() ; it != m_Items.end() ; ++it )
	{
		Item* Item = ( *it );

		if( !FogOfWarExistsAtPosition( Item->GetPosition().x, Item->GetPosition().y ) )
		{
			Item->Draw();
		}

		else if( FogOfWarExistsAtPosition( Item->GetPosition().x, Item->GetPosition().y ) )
		{
			m_questionMark.Draw( Globals::screen, Item->GetPosition () );
		}
	}

	for( std::vector<Monster*>::iterator it = m_monsters.begin() ; it != m_monsters.end() ; ++it )
	{
		Monster* monster = ( *it );
		Player* player = Globals::player;
		int distance = static_cast<int>( Tools::CalculateDistance( monster->GetPosition().x, monster->GetPosition().y, player->GetPosition().x, player->GetPosition().y ) );

		if( player->GetSkillOfType( skillTypes::INCREASED_VISIBILITY ).SkillIsLearned() )
		{
			if( distance < DISTANCE_FROM_PLAYER_TO_RENDER + 3 ) 
			{
				monster->Draw();
			}
		}

		else
		{
			if( distance < DISTANCE_FROM_PLAYER_TO_RENDER ) 
			{
				monster->Draw();
			}
		}

		/*if( !FogOfWarExistsAtPosition( monster->GetPosition().x, monster->GetPosition().y ) && Globals::camera->InCameraView( monster ) )
		{
		monster->Draw();
		}

		else if( FogOfWarExistsAtPosition( monster->GetPosition().x, monster->GetPosition().y ) && Globals::camera->InCameraView( monster ) )
		{
		m_questionMark.Draw( Globals::screen, monster->GetPosition() );
		}	*/
	}

	//Grafika fog of war zamula grę i pozostaniemy przy "próżni" SDL'a :D
	for( std::vector<Tile*>::iterator it = m_tiles.begin() ; it != m_tiles.end() ; ++it )
	{
		Tile* tile = ( *it );
		if( Globals::camera->InCameraView( tile ) )
		{
			//tile->GetFogOfWar().Draw();
		}
	}
}

void Map::LoadTiles( void )
{
	std::ifstream mapTiles;
	mapTiles.open( ("pliki/mapy/" + m_mapName + "/tiles.txt").c_str() );

	if( mapTiles )
	{
		std::vector<TileInfo> tileInfoVector;

		while( !mapTiles.eof() )
		{
			TileInfo tileInfo;

			mapTiles >> tileInfo.m_position.x;
			mapTiles >> tileInfo.m_position.y;
			mapTiles >> tileInfo.m_spriteName;

			tileInfoVector.push_back( tileInfo );
		}

		for( std::vector<TileInfo>::const_iterator cit = tileInfoVector.begin() ; cit != tileInfoVector.end() ; ++cit )
		{
			m_tiles.push_back( Globals::factory->CreateTile( Position( cit->m_position.x, cit->m_position.y), cit->m_spriteName ) );
		}

		mapTiles.close();
	}
}

void Map::LoadStaticMapItems( void )
{
	std::ifstream staticMapItems;
	staticMapItems.open( ( "pliki/mapy/" + m_mapName + "/staticmapitems.txt" ).c_str() );

	if( staticMapItems )
	{
		std::vector<StaticMapItemInfo> staticMapItemInfoVector;

		while( !staticMapItems.eof() )
		{
			StaticMapItemInfo staticMapItemInfo;

			staticMapItems >> staticMapItemInfo.m_position.x;
			staticMapItems >> staticMapItemInfo.m_position.y;
			staticMapItems >> staticMapItemInfo.m_name;
			staticMapItems >> staticMapItemInfo.m_walkable;
			staticMapItems >> staticMapItemInfo.m_spriteName;

			staticMapItemInfoVector.push_back( staticMapItemInfo );
		}

		for( std::vector<StaticMapItemInfo>::const_iterator cit = staticMapItemInfoVector.begin() ; cit != staticMapItemInfoVector.end() ; ++cit )
		{
			// cit->m_walkable
			// warning C4800: 'const int' : forcing value to bool 'true' or 'false' (performance warning)
			m_staticItems.push_back( Globals::factory->CreateStaticMapItem( Position( cit->m_position.x, cit->m_position.y ), cit->m_name, cit->m_walkable, cit->m_spriteName ) );
		}

		staticMapItems.close();
	}
}

void Map::LoadActionMapChange( void )
{
	std::string filepath = "pliki/mapy/" + m_mapName + "/change_location_positions.json";
	std::ifstream file( filepath );

	if (file.is_open())
	{
		nlohmann::json j;
		file >> j;

		for (auto& map : j["maps"])
		{
			for (auto& pos : map["positions"])
			{
				m_actionMapChangeList.push_back( new ActionMapChange( Position( pos["x"], pos["y"] ), map["name"], Position( pos["next_x"], pos["next_y"] ) ) );
			}
		}

		file.close();
	}
}

void Map::LoadMonsters( void )
{
	std::string filepath = "pliki/mapy/" + m_mapName + "/monster_respawns.json";
	std::ifstream file( filepath );

	if( file.is_open() )
	{
		nlohmann::json j;
		file >> j;

		for( auto& monster : j["monsters"] )
		{
			m_monsters.push_back( Globals::factory->CreateMonsterRespawn(
				monster["name"],
				Position( monster["location"]["x"], monster["location"]["y"] ),
				monster["respawn_time"]
			) );
		}

		file.close();
	}
}

void Map::LoadStartExitPositions( void )
{
	std::ifstream stream;
	stream.open( ( "pliki/mapy/" + m_mapName + "/startposition.txt" ).c_str() );

	if( stream )
	{
		while( !stream.eof() )
		{
			stream >> m_startPosition.x >> m_startPosition.y;
		}
	}

	stream.close();

	stream.open( ( "pliki/mapy/" + m_mapName + "/exitposition.txt" ).c_str() );

	if( stream )
	{
		while( !stream.eof() )
		{
			stream >> m_exitPosition.x >> m_exitPosition.y;
		}
	}

	stream.close();
}

void Map::LoadMapEvents( void )
{
	//TODO...
}

bool Map::TileExistsAtPosition( int x, int y )
{
	for( std::vector<Tile*>::const_iterator cit = m_tiles.begin() ; cit != m_tiles.end() ; ++cit )
	{
		if( x == ( *cit )->GetPosition().x && y == ( *cit )->GetPosition().y )
		{
			return true;
		}
	}

	return false;
}

bool Map::StaticItemExistsAtPosition( int x, int y )
{
	for ( std::vector<StaticMapItem*>::const_iterator cit = m_staticItems.begin() ; cit != m_staticItems.end() ; ++cit )
	{
		if( x == ( *cit )->GetPosition().x && y == ( *cit )->GetPosition().y )
		{
			return true;
		}
	}

	return false;
}

bool Map::MonsterExistsAtPosition( int x, int y )
{
	for( std::vector<Monster*>::const_iterator cit = m_monsters.begin() ; cit != m_monsters.end() ; ++cit )
	{
		if( x == ( *cit )->GetPosition().x && y == ( *cit )->GetPosition().y )
		{
			return true;
		}
	}

	return false;
}

bool Map::ItemExistsAtPosition( int x, int y )
{
	for( std::vector<Item*>::const_iterator cit = m_Items.begin() ; cit != m_Items.end() ; ++cit )
	{
		if( x == ( *cit )->GetPosition().x && y == ( *cit )->GetPosition().y )
		{
			return true;
		}
	}

	return false;
}

bool Map::FogOfWarExistsAtPosition( int x, int y )
{
	for( std::vector<Tile*>::iterator it = m_tiles.begin() ; it != m_tiles.end() ; ++it )
	{
		Tile* tile = ( *it );
		if( tile->GetFogOfWar().GetPosition().x == x && tile->GetFogOfWar().GetPosition().y == y && !tile->GetFogOfWar().IsExplored() )
		{
			return true;
		}
	}

	return false;
}

void Map::RemoveStaticMapItem( int x, int y )
{
	std::vector<StaticMapItem*>& staticMapItems = GetStaticMapItemVector();
	std::vector<StaticMapItem*>::iterator cit;

	for( cit = staticMapItems.begin() ; cit != staticMapItems.end() ; )
	{
		if( ( *cit )->GetPosition().x == x && ( *cit )->GetPosition().y == y )
		{
			delete ( *cit );
			cit = staticMapItems.erase( cit );
		}

		else
			++cit;
	}
}

void Map::MoveAllMonsters( void )
{
	for( std::vector<Monster*>::const_iterator cit = m_monsters.begin() ; cit != m_monsters.end() ; ++cit )
	{
		( *cit )->RandomMovement();
	}
}
