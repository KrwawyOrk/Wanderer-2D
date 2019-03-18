#ifndef ITEMSLOT_H
#define ITEMSLOT_

#include <string>

#include "Button.h"

class Item;
class Sprite;

class ItemSlot : public Button
{
public:
	ItemSlot( int x, int y );

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
	Sprite m_selectedSprite;
};

#endif