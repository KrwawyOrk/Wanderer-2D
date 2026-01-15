// Particle.h
#ifndef PARTICLE_H
#define PARTICLE_H

#include "SDL.h"
#include <cstdlib>
#include <cmath>

enum ParticleType {
    BLOOD,        // Czerwona krew (człowiek)
    GREEN_BLOOD,  // Zielona maź (roboty/pająki)
    SPARKS,       // Iskry (elektryczne/metaliczne)
    SMOKE         // Dym (opcjonalnie)
    // Dodaj więcej: SLIME, FIRE, etc.
};

class Particle {
public:
    ParticleType type;
    float x, y;           // pozycja w świecie
    float vx, vy;         // prędkość
    float life;           // 1.0 → 0.0
    float maxLife;
    float size;
    Uint8 r, g, b;        // kolor (dla krwi: czerwony)

    Particle( float startX, float startY, ParticleType pType )
        : x( startX ), y( startY ), life( 1.0f ), type( pType )
    {
        float angle = (rand() % 628) / 100.0f;

        switch (type) {
        case BLOOD: {
            float speed = 120.0f + (rand() % 130);
            float angle = (rand() % 628) / 100.0f;
            vx = cosf( angle ) * speed;
            vy = sinf( angle ) * speed - 80.0f;
            size = 3.5f + (rand() % 35) / 10.0f;
            maxLife = 0.6f + (rand() % 40) / 100.0f;
            r = 140 + (rand() % 115); g = 5 + (rand() % 25); b = 5;
            break;
        }

        case GREEN_BLOOD: {
            float speed = 100.0f + (rand() % 100);
            float angle = (rand() % 628) / 100.0f;
            vx = cosf( angle ) * speed;
            vy = sinf( angle ) * speed - 100.0f;
            size = 4.0f + (rand() % 30) / 10.0f;
            maxLife = 0.8f + (rand() % 20) / 100.0f;
            r = 40 + (rand() % 60); g = 160 + (rand() % 95); b = 40;
            break;
        }

        case SPARKS: {
            float speed = 200.0f + (rand() % 150);
            float angle = (rand() % 628) / 100.0f;
            vx = cosf( angle ) * speed;
            vy = sinf( angle ) * speed - 150.0f;
            size = 2.0f + (rand() % 20) / 10.0f;
            maxLife = 0.3f + (rand() % 10) / 100.0f;
            r = 255; g = 200 + (rand() % 55); b = 50;
            break;
        }

        case SMOKE: {
            float speed = 40.0f + (rand() % 40);
            float angle = (rand() % 628) / 100.0f;
            vx = cosf( angle ) * speed;
            vy = sinf( angle ) * speed - 120.0f;
            size = 5.0f + (rand() % 30) / 10.0f;
            maxLife = 1.2f;
            r = 80 + (rand() % 60); g = 70 + (rand() % 50); b = 60;
            break;
            break;
        }
        }
    }

    void Update( float deltaTime )
    {
        x += vx * deltaTime;
        y += vy * deltaTime;

        switch (type) {
        case BLOOD:      vy += 350.0f * deltaTime; vx *= 0.94f; break;
        case GREEN_BLOOD: vy += 250.0f * deltaTime; vx *= 0.96f; break;  // Lżejsza
        case SPARKS:     vy += 150.0f * deltaTime; vx *= 0.92f; break;  // Szybka
        case SMOKE:      vy -= 50.0f * deltaTime;  vx *= 0.98f; break;  // Unosi się!
        }

        life -= deltaTime / maxLife;
        size *= 0.96f;
    }

    bool IsDead() const { return life <= 0.0f; }

    // Rysowanie kółka (proste, ale ładne)
    void Draw( SDL_Surface* screen, int camX, int camY )
    {
        if (IsDead()) return;

        int screenX = static_cast<int>(x - camX);
        int screenY = static_cast<int>(y - camY);
        Uint8 alpha = static_cast<Uint8>(life * 255);

        Uint32 color = SDL_MapRGBA( screen->format, r, g, b, alpha );

        int radius = static_cast<int>(size);
        for (int dy = -radius; dy <= radius; ++dy)
        {
            for (int dx = -radius; dx <= radius; ++dx)
            {
                if (dx * dx + dy * dy <= radius * radius)
                {
                    int px = screenX + dx;
                    int py = screenY + dy;
                    if (px >= 0 && px < screen->w && py >= 0 && py < screen->h)
                    {
                        // Proste putpixel (działa w SDL 1.2)
                        Uint32* pixels = (Uint32*)screen->pixels;
                        pixels[py * screen->w + px] = color;
                    }
                }
            }
        }
    }
};

#endif