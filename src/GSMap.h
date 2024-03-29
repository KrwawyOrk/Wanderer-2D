#ifndef GSMAP_H
#define GSMAP_H

#include "GameState.h"
#include "Sprite.h"

class Button;

class GSMap : public GameState
{
public:
	GSMap();
	virtual ~GSMap();

	virtual void InputEvents( void );

	virtual void Think( void );

	virtual void Update( float deltaTime )
	{
		//...
	}

	virtual void Draw( void );

	virtual void OnQuit( void )
	{
		//...
	}

private:
	Sprite m_map;
	Button* m_exitButton;
	Button* m_secretLaboratory;
};

#endif
