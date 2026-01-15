#ifndef GSPlaying_H
#define GSPlaying_H

#include "GameState.h"
#include "BitmapFont.h"
#include "Particle.h"
#include "Player.h"
#include "PlayerBelt.h"
#include "Map.h"
#include "Sprite.h"

#include <list>
#include <string>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

enum playingState_t
{
    GAMEPLAY = 1,
	BUILDING
};

class Container;
class Item;
class NPC;
class Particle;
class TextBox;

class GSPlaying : public GameState
{
public:
	GSPlaying();
	virtual ~GSPlaying();

	virtual void InputEvents( void );

	virtual void Think( void );
	void ThinkMovementInputButtonsHeld( void );

	virtual void Update( float deltaTime );
	virtual void Draw( void );
	virtual void OnQuit( void ) { return; }

	Player* GetPlayer( void ) { return m_player; }

	PlayerBelt& GetPlayerBelt( void ) { return m_playerBelt; }

	Map* GetMapByName( std::string mapName );
	Map* GetCurrentMap( void ) { return m_map; }
	bool MapExists( std::string mapName ) const;
	void ChangeMap( const std::string& mapName );
	void PlaceProtectionZone( Position& position );
	int GetDay( void ) { return m_day; }
	void PlayerLeaveMap( void );
	void DrawExitLocationMessage( void );
	void DrawOpenedContainer( void );
	void CheckIfContainerIsOpened( void );
	void OpenMapEditorToEditCurrentMap( void );
	void OpenMapEditorNewMapWindow( void );
	void PrintInformationsInConsole( void );
	void MoveCameraOnMouseMotion( void );
	void ShakeScreen( SDL_Surface* screen, int shake_count, int shake_intensity );
	void HandleMouseClickMapActions( int mouse_x, int mouse_y, std::vector<json>& actions );
	void FadeToBlack( SDL_Surface* screen, int fadeTimeMs );
	void EmitParticles( float worldX, float worldY, ParticleType type, int count = 18 );
	void DrawParticles( void );
	void DrawScene( void );
	void DrawUI( void );

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

	TextBox* m_informationsConsole;

	std::vector<Particle*> m_particles;

private:
	// --- Bateria latarki ---
	float m_flashlightBattery;      // 0.0f – 1.0f

	// --- Maski œwiat³a ---
	SDL_Surface* m_flashlightMask;
	int m_currentMaskRadius;

	// --- Funkcje zarz¹dzania bateri¹ ---
	void UpdateFlashlightBattery( float deltaTime );
	bool IsFlashlightOn() const;
	float GetFlashlightBatteryLevel() const { return m_flashlightBattery; }
	int GetFlashlightRadius() const;

	// --- Funkcje zarz¹dzania mask¹ ---
	void EnsureFlashlightMask( int radius );
	void ReleaseFlashlightMask();
	SDL_Surface* CreateFlashlightMask( int radius ) const;
};

#endif
