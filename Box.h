#ifndef BOX_H
#define BOX_H

#include "GameObject.h"

class Box : public GameObject
{
public:
	Box();

	virtual void Think( void ) { return; }
	virtual void Update( float deltaTime) { return; }
};

#endif
