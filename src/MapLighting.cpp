#include "MapLighting.h"
#include <cmath>
#include <algorithm>

#include "Camera.h"
#include "Globals.h"

void MapLighting::AddLight( const Light& light )
{
    m_lights.push_back( light );
}

void MapLighting::ClearCache()
{
    for (auto& pair : m_maskCache) {
        if (pair.second) {
            SDL_FreeSurface( pair.second );
        }
    }
    m_maskCache.clear();
}

SDL_Surface* MapLighting::GetCachedMask( int radius ) const
{
    auto it = m_maskCache.find( radius );
    if (it != m_maskCache.end()) {
        return it->second;
    }

    // Stwórz nową maskę
    int size = radius * 2;
    SDL_Surface* mask = SDL_CreateRGBSurface( SDL_SWSURFACE, size, size, 32,
        0x000000FF, 0x0000FF00, 0x00FF0000, 0 );

    if (!mask) return nullptr;

    Uint32* pixels = (Uint32*)mask->pixels;
    int pitch = mask->pitch / sizeof( Uint32 );

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            int dx = x - radius;
            int dy = y - radius;
            if (dx * dx + dy * dy <= radius * radius) {
                float dist = std::sqrt( (float)(dx * dx + dy * dy) );
                float t = 1.0f - (dist / (float)radius);
                t = std::pow( std::max( t, 0.0f ), 1.8f );
                Uint8 intensity = (Uint8)(t * 255);
                pixels[y * pitch + x] = SDL_MapRGB( mask->format, intensity, intensity, intensity );
            }
        }
    }

    m_maskCache[radius] = mask;
    return mask;
}

void MapLighting::Render( SDL_Surface* sceneBuffer, SDL_Surface* target )
{
    // Opcjonalnie: wyczyść ekran? Nie — załóżmy, że jest już czarny
    for (const auto& light : m_lights) {
        if (!light.isActive) continue;

        // ✅ Sprawdź, czy światło jest w zasięgu kamery
        float camX = Globals::camera->GetCameraX();
        float camY = Globals::camera->GetCameraY();
        float camW = (float)target->w;
        float camH = (float)target->h;

        if (light.x + light.radius < camX) continue;
        if (light.x - light.radius > camX + camW) continue;
        if (light.y + light.radius < camY) continue;
        if (light.y - light.radius > camY + camH) continue;

        if (light.shape == LightShape::LIGHT_CIRCLE) {
            RenderCircleLight( sceneBuffer, target, light );
        }
    }
}

void MapLighting::RenderCircleLight( SDL_Surface* scene, SDL_Surface* target, const Light& light )
{
    SDL_Surface* mask = GetCachedMask( light.radius );
    if (!mask) return;

    // ✅ Konwersja świata → ekranu
    int screenX = (int)(light.x - Globals::camera->GetCameraX() );
    int screenY = (int)(light.y - Globals::camera->GetCameraY() );

    BlendLight( scene, target, mask,
        screenX - light.radius,
        screenY - light.radius,
        light.r, light.g, light.b );
}

void MapLighting::BlendLight( SDL_Surface* scene, SDL_Surface* target, SDL_Surface* mask,
    int dstX, int dstY, Uint8 r, Uint8 g, Uint8 b )
{
    int w = mask->w;
    int h = mask->h;

    // Obsługa krawędzi
    int srcX = 0, srcY = 0;
    if (dstX < 0) { srcX = -dstX; w -= srcX; dstX = 0; }
    if (dstY < 0) { srcY = -dstY; h -= srcY; dstY = 0; }
    if (dstX + w > target->w) w = target->w - dstX;
    if (dstY + h > target->h) h = target->h - dstY;

    if (w <= 0 || h <= 0) return;

    SDL_LockSurface( target );
    SDL_LockSurface( scene );
    SDL_LockSurface( mask );

    Uint32* screen = (Uint32*)target->pixels;
    Uint32* scenePixels = (Uint32*)scene->pixels;
    Uint32* maskPixels = (Uint32*)mask->pixels;

    int sp = target->pitch / sizeof( Uint32 );
    int bp = scene->pitch / sizeof( Uint32 );
    int mp = mask->pitch / sizeof( Uint32 );

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            Uint32 scenePixel = scenePixels[(dstY + y) * bp + (dstX + x)];
            Uint32 maskPixel = maskPixels[(srcY + y) * mp + (srcX + x)];

            Uint8 sr, sg, sb;
            SDL_GetRGB( scenePixel, scene->format, &sr, &sg, &sb );

            Uint8 intensity;
            SDL_GetRGB( maskPixel, mask->format, &intensity, &intensity, &intensity );

            // Najpierw: ile sceny widać
            sr = (sr * intensity) / 255;
            sg = (sg * intensity) / 255;
            sb = (sb * intensity) / 255;

            // Potem: pomnóż przez kolor światła
            sr = (sr * r) / 255;
            sg = (sg * g) / 255;
            sb = (sb * b) / 255;

            screen[(dstY + y) * sp + (dstX + x)] = SDL_MapRGB( target->format, sr, sg, sb );
        }
    }

    SDL_UnlockSurface( mask );
    SDL_UnlockSurface( scene );
    SDL_UnlockSurface( target );
}