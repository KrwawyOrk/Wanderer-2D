#ifndef FOODGENERATOR_H
#define FOODGENERATOR_H

#include "StaticMapItem.h"
#include "Text.h"

class Player;

class FoodGenerator : public StaticMapItem
{
public:
	FoodGenerator( Position position );
	~FoodGenerator() {}

	void PlayerWalkOnFoodGenerator( Player* player );
	void NextTurn( void );

private:
	int m_turnsPassed;
	bool m_food;
};

#endif

