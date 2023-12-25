#ifndef GSBATTLEFIGHT_H
#define GSBATTLEFIGHT_H

#include "GameState.h"

#include "Button.h"
#include "Player.h"
#include "Sprite.h"

class GSBattleFight : public GameState
{
public:
	GSBattleFight();
	virtual ~GSBattleFight();

	virtual void InputEvents( void );
	virtual void Think( void );
	virtual void Update( float deltaTime );
	virtual void Draw( void );
	virtual void OnQuit( void );

	void Attack( void );

private:
	Player* m_player;
	Sprite m_background;

	Button* m_attackButton;
};

#endif