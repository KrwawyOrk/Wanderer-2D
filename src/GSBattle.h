#ifndef GSBATTLE_H
#define GSBATTLE_H

#include "GameState.h"
#include "Sprite.h"

class Creature;
class Player;

class GSBattle : public GameState
{
public:
	GSBattle()
	{
		LoadFiles();
	}

	virtual ~GSBattle()
	{
		Destroy();
	}

	virtual void LoadFiles( void );
	virtual void Destroy( void ) { return; }

	virtual void InputEvents( void );
	virtual void Think( void ) { return; }
	virtual void Draw( void );

	void StartBattle( Player* player, Creature* attackedCreature );

private:
	Sprite m_background;
};

#endif
