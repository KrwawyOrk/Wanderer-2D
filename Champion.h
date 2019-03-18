#ifndef CHAMPION_H
#define CHAMPION_H

#include "Creature.h"

class Champion : public Creature
{
public:
	Champion();
	virtual ~Champion(void);

	virtual void Think( void ) { return;}
	virtual void Update( float deltaTime ) { return; }
	virtual void Draw( void ) { return; }
};

#endif
