#ifndef STATICMAPITEM_H
#define STATICMAPITEM_H

#include "GameObject.h"

class StaticMapItem : public GameObject
{
public:
	StaticMapItem( Position& position, std::string name, bool walkable, std::string spriteName );

	virtual void Think( void ) { return; }
	virtual void Update( float deltaTime ) { return; }

	bool IsWalkable( void ) const
	{
		return m_walkable;
	}

protected:
	bool m_walkable;
};

#endif
