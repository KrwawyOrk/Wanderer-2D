#ifndef GSCRAFTMENU_H
#define GSCRAFTMENU_H

#include "GameState.h"

#include "BitmapFont.h"
#include "Player.h"
#include "Text.h"

class GSCraftMenu : public GameState
{
public:
	GSCraftMenu();
	virtual ~GSCraftMenu();

	virtual void InputEvents( void );
	virtual void Think( void );
	virtual void Update( float deltaTime ) { return; }
	virtual void Draw( void );
	virtual void OnQuit( void );

	void CraftHealthKit( void );
	void CraftToolBox( void );
	void CraftMorphicHammer( void );
	void LearnMiningAbility( void );

private:
	Player* m_player;

	BitmapFont* m_craftRecipes;
	BitmapFont* m_description;

	Text* m_playerItemsText;
	Text* m_craftHealthKitText;
	Text* m_craftToolBoxText;
	Text* m_craftMorphicHammerText;

	Sprite m_background;
	Sprite m_location1;
};

//class CraftItemWindow
//{
//public:
//	CraftItemWindow();
//
//	void Draw( int x, int y );
//
//private:
//	Sprite m_sprite;

#endif