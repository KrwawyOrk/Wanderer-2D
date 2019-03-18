#ifndef PLAYERBELT_H
#define PLAYERBELT_H

#include "Sprite.h"
#include "Text.h"
#include "BitmapFont.h"

#include <vector>

class Button;
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

	Text* m_food;
	Text* m_day;

	BitmapFont* m_healthPoints;
	BitmapFont* m_movementPoints;;
	BitmapFont* m_information;
	BitmapFont* m_toolbox;
	BitmapFont* m_junk;
	BitmapFont* m_pistolAmmunition;
	BitmapFont* m_experiencePoints;
	BitmapFont* m_damage;

	Button* m_inventoryButton;
	Button* m_quitButton;

	std::vector<ItemSlot*> m_itemSlots;
	Sprite m_inventoryBelt;
};

#endif
