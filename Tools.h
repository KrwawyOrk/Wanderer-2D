#ifndef TOOLS_H
#define TOOLS_H

#include <string>

class Tools
{
public:
	static void NumberToString( std::string& string );
	static double CalculateDistance( double dX0, double dY0, double dX1, double dY1 );
};

#endif