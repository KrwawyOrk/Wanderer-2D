#ifndef FOGOFWAR_H
#define FOGOFWAR_H

#include "GameObject.h"
#include "Player.h"

const int FOGOFWAR_DISTANCE = 4;

class FogOfWar : public GameObject
{
public:
	FogOfWar();

	virtual void Think( void );
	virtual void Update( float deltaTime ) {}
	virtual void Draw( void );

	bool IsExplored( void ) const { return m_explored; }
	void SetPlayer( Player* player ) { m_player = player; }

private:
	bool m_explored;
	Player* m_player;
};

#endif

	