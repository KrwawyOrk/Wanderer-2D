#include "FoodGenerator.h"

#include "Globals.h"
#include "Player.h"

FoodGenerator::FoodGenerator( Position position ) : StaticMapItem( Position( position.x, position.y ), "foodgenerator", true, "food_generator" )
{
	m_turnsPassed = 0;
	m_food = false;
	SetPosition( position ); 
}

void FoodGenerator::PlayerWalkOnFoodGenerator( Player* player )
{
	if( m_food && player->GetFood() < player->GetMaxFood() )
	{
		player->GiveFood( 1 );
		m_food = false;
		m_turnsPassed = 0;
	}
}

void FoodGenerator::NextTurn( void )
{
	m_turnsPassed++;

	if( m_turnsPassed == 3 )
	{
		m_food = true;
		m_turnsPassed = 0;
	}
}