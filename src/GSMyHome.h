#ifndef GSMYHOME_H
#define GSMYHOME_H

#include "GameState.h"

#include "Sprite.h"

enum class HomeState
{
	HOME,
	COMPUTER,
	WINDOW
};

class Button;

class GSMyHome : public GameState
{
public:
	GSMyHome();
	~GSMyHome();

	virtual void InputEvents( void );
	virtual void Think( void );
	virtual void Update( float deltaTime );
	virtual void Draw( void );
	virtual void OnQuit( void );

private:
	HomeState m_homeState;
	Sprite m_background;
	Button* m_buttonComputer;
};

#endif

