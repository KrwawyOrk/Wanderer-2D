#ifndef LIGHT_H
#define LIGHT_H

#include "SDL.h"

enum LightShape
{
    LIGHT_CIRCLE
};

struct Light
{
    float x, y;
    int radius;
    Uint8 r, g, b;
    LightShape shape;
    bool isActive;

    Light( float x, float y, int radius, Uint8 r, Uint8 g, Uint8 b )
        : x( x ), y( y ), radius( radius ), r( r ), g( g ), b( b ),
        shape( LIGHT_CIRCLE ), isActive( true )
    {}
};

#endif