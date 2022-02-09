#ifndef STATICMAPITEM_H
#define STATICMAPITEM_H

#include "GameObject.h"

class Container;

class StaticMapItem : public GameObject
{
public:
	StaticMapItem( Position position, std::string name, bool walkable, std::string spriteName );

	virtual void Think( void ) { return; }
	virtual void Update( float deltaTime ) { return; }
	virtual void Draw( void );

	bool IsWalkable( void ) const
	{
		return m_walkable;
	}

	bool HasContainer( void );
	void SetContainer( Container* container );

	Container* GetContainer( void )
	{
		return m_container;
	}

protected:
	bool m_walkable;
	Container* m_container;
};

#endif
