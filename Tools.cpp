#include "Tools.h"

#include <sstream>

void NumberToString( int value, std::string& string )
{
	//TODO	
}

double Tools::CalculateDistance( double dX0, double dY0, double dX1, double dY1 )
{
	return sqrt((dX1 - dX0)*(dX1 - dX0) + (dY1 - dY0)*(dY1 - dY0));
}
