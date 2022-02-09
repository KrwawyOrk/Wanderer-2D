#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Globals.h"
#include "Position.h"
#include "Sprite.h"

#include <string>
#include <vector>
#include <iostream>

enum direction_t
{
	NORTH = 1,
	SOUTH,
	EAST,
	WEST
};

class GameObject
{
public:
	GameObject();
	virtual ~GameObject()  {}

	virtual void Think( void ) = 0;
	virtual void Update( float deltaTime );
	virtual void Draw( void );

	virtual void Move( direction_t direction );

	std::string GetName( void ) const { return m_name; }
	void SetName( std::string name ) { m_name = name; }

	Position& GetPosition( void ) { return m_position; }
	void SetPosition( Position position )
	{
		m_position.x = position.x;
		m_position.y = position.y;
	}

	
	void SetFloatX( float x ) 
	{ 
		flposition_x = x; 
	}

	void SetFloatY( float y ) 
	{ 
		flposition_y = y; 
	}

	float GetFloatX() 
	{ 
		return flposition_x; 
	}

	float GetFloatY() 
	{ 
		return flposition_y; 
	}

	void SetVelocity( float velocity )
	{
		m_velocity = velocity;
	}

	Sprite& GetSprite( void ) { return m_sprite; }
	bool IsMoving( void );
	bool IsMovingUp( void );
	bool IsMovingDown( void );
	bool IsMovingLeft( void );
	bool IsMovingRight( void );

protected:
	std::string m_name;
	Position m_position;
	Sprite m_sprite;

	float flposition_x;
	float flposition_y;
	float m_velocity;
};

#endif
