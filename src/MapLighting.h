#ifndef MAP_LIGHTING_H
#define MAP_LIGHTING_H

#include "Light.h"
#include <vector>
#include <map>

class MapLighting
{
public:
    void AddLight( const Light& light );
    void Render( SDL_Surface* sceneBuffer, SDL_Surface* target );

    // W razie potrzeby: wyczyœæ cache przy zmianie rozdzielczoœci
    void ClearCache();

    std::vector<Light> m_lights;
    mutable std::map<int, SDL_Surface*> m_maskCache; // radius -> maska

    SDL_Surface* GetCachedMask( int radius ) const;
    void RenderCircleLight( SDL_Surface* scene, SDL_Surface* target, const Light& light );
    void BlendLight( SDL_Surface* scene, SDL_Surface* target, SDL_Surface* mask,
        int dstX, int dstY, Uint8 r, Uint8 g, Uint8 b );
};

#endif