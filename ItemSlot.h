#ifndef ITEMSLOT_H
#define ITEMSLOT_

#include <string>

#include "Button.h"

class Item;
class Sprite;

namespace itemSlotType
{
	enum itemSlotType_t
	{
		INVENTORY_SLOT,
		CONTAINER_SLOT
	};
}

class ItemSlot : public Button
{
public:
	ItemSlot( int x, int y, itemSlotType::itemSlotType_t itemSlotType );

	void SetItem( Item* item );
	void RemoveItem( void );

	bool IsSelected( void ) const 
	{
		return m_selected;
	}

	void Selected( bool b ) 
	{
		m_selected = b;
	}

	Item* GetItemSlot( void )
	{
		return m_item;
	}

	bool HasItem( void )
	{
		if( m_item )
		{
			return true;
		}

		else
		{
			return false;
		}
	}

	void Think( void );
	void Draw( void );

private:
	Item* m_item;
	bool m_selected;
	itemSlotType::itemSlotType_t m_itemSlotType;
	Sprite m_selectedSprite;	
};

#endif