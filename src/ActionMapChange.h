#ifndef ACTIONMAPCHANGE_H
#define ACTIONMAPCHANGE_H

#include "GameObject.h"
#include "Position.h"

#include <string>

class ActionMapChange : public GameObject
{
public:
	ActionMapChange( Position position, std::string nextMapName, Position landPosition );

	virtual void Think( void ) {}
	virtual void Update( float deltaTime ) {}

	std::string GetNextMapName() const { return m_nextMapName; }
	//Position GetPosition( void ) const { return m_position; }
	Position GetLandPosition( void ) const { return m_landPosition; }

private:
	//Position m_position;

	std::string m_nextMapName;
	Position m_landPosition;
};

#endif
