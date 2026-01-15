#include "Tools.h"

#include <sstream>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include "Camera.h"
#include "Globals.h"
#include "Position.h"

void NumberToString( int value, std::string& string )
{
	//TODO	
}

double Tools::CalculateDistance( double x1, double y1, double x2, double y2 )
{
	return sqrt( pow( x2 - x1, 2 ) + pow( y2 - y1, 2 ) );
}

int Tools::GenerateRandomNumber( int number )
{
	srand( ( int )time( 0 ) );
	return (rand() % number) + 1;
}

bool Tools::IsMouseClickAtTilePosition( int x, int y )
{
	Position position; //pozycja wcisniecia przycisku myszki
	position.x = (Globals::event.button.x + Globals::camera->GetCameraX()) / Globals::tilesize;
	position.y = (Globals::event.button.y + Globals::camera->GetCameraY()) / Globals::tilesize;

	return( position.x == x && position.y == y );
}