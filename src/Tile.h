#ifndef TILE_H
#define TILE_H

#include "FogOfWar.h"
#include "Position.h"
#include "Sprite.h"

#include <vector>
#include <string>
#include <iostream>

class Tile
{
public:
	Tile() {}
	Tile( Position& position, std::string spriteName );

	~Tile()
	{
		std::cout << "Niszcze obiekt klasy Tile." << std::endl;
	}

	void SetPosition( Position& position )
	{
		m_position.x = position.x;
		m_position.y = position.y;
	}


	Sprite& GetSprite( void ) { return m_sprite; }
	Position& GetPosition( void ) { return m_position; }

	FogOfWar& GetFogOfWar( void ) { return m_fogofwar; }

	void Think( void );
	void Draw( void );

	void SetProtectionZone( bool b )
	{
		m_protectionZone = b;
	}

	bool IsProtectionZone( void ) 
	{ 
		return m_protectionZone; 
	}


private:
	Position m_position;
	Sprite m_sprite;
	Sprite m_protectionZoneSprite;
	FogOfWar m_fogofwar;
	bool m_protectionZone;
};

#endif
