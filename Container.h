#ifndef CONTAINER_H
#define CONTAINER_H

#include <string>
#include <vector>

#include "StaticMapItem.h"

class Button;
class Item;
class ItemSlot;
class Sprite;

class Container
{
public:
	Container();
	~Container();

	void InputEvents( void );
	void Think( void );
	void Draw( void );
	
	std::vector<ItemSlot*>& GetItemSlots( void )
	{
		return m_itemSlots;
	}

	void AddItem( Item* item );
	bool HasSpaceInContainer( void );

	void SetContainerLocked( bool b )
	{
		m_locked = b;
	}

	bool IsLocked( void )
	{
		return m_locked;
	}

private:
	std::vector<ItemSlot*> m_itemSlots;

	bool m_locked;
	Button* m_exitButton;
	Sprite m_background;
};

#endif