#ifndef PLAYERBELT_H
#define PLAYERBELT_H

#include "Sprite.h"
#include "Text.h"
#include "BitmapFont.h"

#include <vector>

class Button;
class Container;
class Item;
class ItemSlot;
class Player;

class PlayerBelt
{
public:
	PlayerBelt();
	~PlayerBelt();

	void InputEvents( void );
	void Think( void );
	void Draw( void );

	void SetPlayer( Player* player )
	{
		if( player )
		{
			m_player = player;
		}
	}

	void InsertItemToInventory( Item* item );
	void DrawPlayerHealthBar( int position_x, int position_y );

	std::vector<ItemSlot*>& GetItemSlots( void ) { return m_itemSlots; }

private:
	Player* m_player;

	Sprite m_background;
	Sprite m_healthPotion;
	Sprite m_belt1;

	Sprite m_top;
	Sprite m_bottom;
	Sprite m_left;
	Sprite m_right;
	Sprite m_detail01;

	Sprite m_pistol;
	Sprite m_crowbar;

	Sprite m_playerHealthBar;
	Sprite m_gui_background;

	Text* m_food;
	Text* m_day;

	BitmapFont* m_healthPoints;
	BitmapFont* m_toolbox;
	BitmapFont* m_junk;
	BitmapFont* m_pistolAmmunition;
	BitmapFont* m_experiencePoints;
	BitmapFont* m_damage;
	BitmapFont* m_monstersKilled;

	Button* m_inventoryButton;
	Button* m_quitButton;

	std::vector<ItemSlot*> m_itemSlots;
	Sprite m_inventoryBelt;

	ItemSlot* m_containerSlotTest;
	Container* m_container;
};

#endif
