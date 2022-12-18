#include "Tools.h"

#include <sstream>
#include <cstdlib>
#include <cmath>
#include <ctime>

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
