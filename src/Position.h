#ifndef POSITION_H
#define POSITION_H

class Position
{
public:
	Position( int x, int y ) : x(x), y(y) {}
	Position() : x(0), y(0) {}

	int x;
	int y;
};

#endif