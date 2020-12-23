#ifndef MAP_H
#define MAP_H

#include "ActionMapChange.h"
#include "Box.h"
#include "Monster.h"
#include "NPC.h"
#include "Position.h"
#include "Sprite.h"
#include "StaticMapItem.h"
#include "Tile.h"
#include "Item.h"
#include "FoodGenerator.h"

#include <list>
#include <string>
#include <vector>

const int DISTANCE_FROM_PLAYER_TO_RENDER = 3;

class CollisionTile
{
public:
	CollisionTile( Position& position );

	Position& GetPosition( void )
	{
		return m_position;
	}

private:
	Position m_position;
};

class Map
{
public:
	Map( std::string mapName );
	~Map();

	void Think( void );
	void Update( float deltaTime );
	void Draw( void );

	std::string GetMapName( void ) const { return m_mapName; }
	bool TileExistsAtPosition( int x, int y);
	bool StaticItemExistsAtPosition( int x, int y );
	bool MonsterExistsAtPosition( int x, int y );
	bool FogOfWarExistsAtPosition( int x, int y );
	bool ItemExistsAtPosition( int x, int y );
	bool FoodGeneratorExistsAtPosition( int x, int y );
	void GenerateRandomObjectsOnMap( void );
	void RemoveStaticMapItem( int x, int y );

	void TestRandomNumbers( void );
	void MoveAllMonsters( void );

	std::vector<Tile*>& GetTilesVector( void ) { return m_tiles; }
	std::vector<Monster*>& GetMonstersVector( void ) { return m_monsters; }
	std::vector<StaticMapItem*>& GetStaticMapItemVector( void ) { return m_staticItems; }
	std::vector<Box*>& GetBoxes( void ) { return m_boxes; } 
	std::vector<Item*>& GetItemsVector( void ) { return m_Items; }
	std::vector<FoodGenerator*>& GetFoodGeneratorsVector( void ) { return m_foodGenerators; }
	std::list<ActionMapChange*>& GetActionMapChangeList( void ) { return m_actionMapChangeList; }

	Position GetStartPosition( void )
	{
		return m_startPosition;
	}

	Position GetExitPosition( void )
	{
		return m_exitPosition;
	}

private:
	struct TileInfo
	{
		Position m_position;
		std::string m_spriteName;
	};

	struct StaticMapItemInfo
	{
		Position m_position;
		std::string m_name;
		bool m_walkable; //zmienione z int na bool
		std::string m_spriteName;
	};

	struct ActionMapChangeInfo
	{
		Position m_position;
		std::string m_nextMapName;
		Position m_landPosition;
	};

	struct MonsterRespawnInfo
	{
		std::string m_monsterName;
		Position m_respawnPosition;
		int m_respawnTime;
		int m_deathTime;
	};

	void LoadTiles( void );
	void LoadStaticMapItems( void );
	void LoadActionMapChange( void );
	void LoadMonsters( void );
	void LoadStartExitPositions( void );

	std::vector<Tile*> m_tiles;
	std::vector<StaticMapItem*> m_staticItems;
	std::vector<Monster*> m_monsters;
	std::vector<MonsterRespawnInfo*> m_monstersWaitingForRespawn;
	std::vector<Box*> m_boxes;
	std::vector<Item*> m_Items;
	std::vector<FoodGenerator*> m_foodGenerators;
	std::list<ActionMapChange*> m_actionMapChangeList;
	std::vector<NPC*> m_npcs;

	Sprite m_questionMark;
	std::string m_mapName;

	Position m_startPosition;
	Position m_exitPosition;
	Sprite m_exitSprite;
};

#endif