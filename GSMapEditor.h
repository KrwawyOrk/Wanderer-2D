#ifndef GSMAPEDITOR_H
#define GSMAPEDITOR_H

#include "Button.h"
#include "GameState.h"
#include "GUITextInput.h"
#include "Position.h"
#include "Sprite.h"

#include "SDL.h"
#include <iostream>

enum tilemode_t
{
	TILE = 1,
	STATIC_ITEM,
	MONSTER,
	PROTECTION_ZONE
};

enum
{
	TILE_YES,
	TILE_NO,
	STATICMAPITEM_YES,
	STATICMAPITEM_NO,
	WALKABLE_YES,
	WALKABLE_NO,
};

enum mapEditorState_t
{
	MAPPING,
	MENU,
	SPRITES
};

class BitmapFont;
class Map;

class GSMapEditor : public GameState
{
public:
	enum brushDirection_t
	{
		UP = 1,
		DOWN,
		LEFT,
		RIGHT
	};

	GSMapEditor();
	virtual ~GSMapEditor();

	virtual void InputEvents( void );
	virtual void Think( void ) ;
	virtual void Update( float deltaTime ) { return; }
	virtual void Draw( void );
	virtual void OnQuit( void ) { return; }

	Position& GetBrushPosition( void ) { return m_brushPosition; }
	void SetBrushPosition( Position& position )
	{
		m_brushPosition.x = position.x;
		m_brushPosition.y = position.y;
	}

	void MoveBrush( brushDirection_t brushDirection );
	void SaveMap( Map* map );

	void AddTile( Map* map, Position& position, std::string spriteName );
	void AddStaticMapItem( Map* map, Position& position, bool walkable, std::string spriteName );

	void RemoveTile( Map* map, Position& position );
	void RemoveStaticMapItem( Map* m_map, Position& position );

	void SetMapEditorState( mapEditorState_t mapEditorState )
	{
		m_mapEditorState = mapEditorState;
	}

	void GiveMap( Map* map ) //edycja mapy na ktorej gramy
	{
		m_map = map;
	}

	void DrawMousePositionInfo( void );
	void DrawBrush( void );

	void InputMappingState( void );
	void InputSpriteBrowserState( void );
	void InputMenuState( void );

	void ThinkMappingState( void );
	void ThinkSpriteBrowserState( void );
	void ThinkMenuState( void );

	void DrawMappingState( void );
	void DrawSpriteBrowserState( void );
	void DrawMenuState( void );

private:
	Position m_brushPosition;
	Sprite m_brushSprite;
	int m_brushSize;

	bool m_keysHeld[323];
	bool m_mouseHeld; //lewy przycisk myszki
	bool m_mouseHeldRight;

	GUITextInput* m_textInput;
	GUITextInput* m_mapNameInput;

	Map* m_map;

	Sprite m_selectedSprite;
	Sprite m_mapEditorGraphics[6];
	std::string m_selectedSpriteName;

	mapEditorState_t m_mapEditorState;
	tilemode_t m_mode;
	bool m_walkableStaticMapItem;
	bool m_showModeInfo;
	BitmapFont* m_mousePositionInfo;
	BitmapFont* m_spriteName;
	bool m_mapEditorNotifications;

	Sprite m_backgroundPaper;

	std::vector<Button*> m_buttonsVector;
};


#endif

