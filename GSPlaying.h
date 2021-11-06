#ifndef GSPlaying_H
#define GSPlaying_H

#include "GameState.h"
#include "BitmapFont.h"
#include "Player.h"
#include "PlayerBelt.h"
#include "Map.h"
#include "Sprite.h"

#include <list>
#include <string>

enum playingState_t
{
    GAMEPLAY = 1,
	BUILDING
};

class Container;
class FoodGenerator;
class Item;
class NPC;

class GSPlaying : public GameState
{
public:
	GSPlaying();
	virtual ~GSPlaying();

	virtual void InputEvents( void );
	virtual void Think( void );
	virtual void Update( float deltaTime );
	virtual void Draw( void );
	virtual void OnQuit( void ) { return; }

	Player* GetPlayer( void ) { return m_player; }

	PlayerBelt& GetPlayerBelt( void ) { return m_playerBelt; }

	Map* GetMapByName( std::string mapName );
	Map* GetCurrentMap( void ) { return m_map; }
	bool MapExists( std::string mapName ) const;
	void ChangeMap( std::string mapName );
	void NextTurn( void );
	void PlaceProtectionZone( Position& position );
	void PlaceFoodGenerator( Position& position );
	int GetDay( void ) { return m_day; }
	void PlayerLeaveMap( void );
	void DrawExitLocationMessage( void );
	void DrawOpenedContainer( void );
	void CheckIfContainerIsOpened( void );

private:
	bool m_keysHeld[323];
    playingState_t m_playingState;
	PlayerBelt m_playerBelt;

	Map* m_map;
	std::list<Map*> m_mapList;

	Player* m_player;
	GameObject* m_flying;

	bool m_fishing;
	Sprite m_fishingSprite;

	int m_day;
	BitmapFont* m_exitLocationMessage;

	Container* m_openedContainer;
	NPC* m_npc;
};

#endif
