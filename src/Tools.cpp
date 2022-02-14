#include "Tools.h"

#include <sstream>
#include <cstdlib>
#include <cmath>
#include <ctime>

void NumberToString( int value, std::string& string )
{
	//TODO	
}

double Tools::CalculateDistance( double dX0, double dY0, double dX1, double dY1 )
{
	return sqrt((dX1 - dX0)*(dX1 - dX0) + (dY1 - dY0)*(dY1 - dY0));
}

int Tools::GenerateRandomNumber( int number )
{
	srand( ( int )time( 0 ) );
	return (rand() % number) + 1;
}
