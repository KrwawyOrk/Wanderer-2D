#ifndef TOOLS_H
#define TOOLS_H

#include <string>

class Tools
{
public:
	static void NumberToString( std::string& string );
	static double CalculateDistance( double x1, double y1, double x2, double y2 );
	static int GenerateRandomNumber( int number );
};

#endif