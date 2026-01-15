#ifndef MAPLOGIC_H
#define MAPLOGIC_H

class MapLogic
{
	virtual void InputEvents( void ) = 0;
	virtual void Think( void ) = 0;
	virtual void Update( float deltaTime ) = 0;
	virtual void Draw( void ) = 0;

	virtual void OnQuit( void ) = 0;
};

#endif

