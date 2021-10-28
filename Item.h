#ifndef Item_H
#define Item_H

#include "GameObject.h"

enum itemType_t
{
	HEALTH_REGENERATION,
	WEAPON,
	EXPERIENCE_BOOK,
	AMMO_PACK
};

class Player;

class Item : public GameObject
{
public:
	Item( Position position, itemType_t itemType );
	~Item();

	virtual void Think( void )
	{
		return;
	}

	virtual void Update( float deltaTime )
	{
		return;
	}

	itemType_t GetItemType()
	{
		return m_itemType;
	}

	bool IsPicked( void )
	{
		return m_picked;
	}

	void Pick( void )
	{
		m_picked = true;
	}

	void OnUse( Player* player );
	void OnEquip( Player* player );
	void OnRemove( Player* player );

private:
	itemType_t m_itemType;
	bool m_picked;
};

#endif