#include "SDL.h"

class ProgressBar {
private:
    SDL_Surface* screen;
    SDL_Rect barRect;
    SDL_Color bgColor;
    SDL_Color fillColor;
    float progress; // 0.0f to 1.0f
    int borderWidth;
    Uint32 startTime; // Start time of animation
    Uint32 duration;  // Duration in milliseconds
    bool isActive;    // Whether the progress bar is animating

public:
    ProgressBar( SDL_Surface* surface, int width, int height )
        : screen( surface ), progress( 0.0f ), borderWidth( 2 ), startTime( 0 ), duration( 0 ), isActive( false ) {
        barRect.w = width;
        barRect.h = height;

        // Default colors (RGB)
        bgColor.r = 50; bgColor.g = 50; bgColor.b = 50;     // Dark gray background
        fillColor.r = 0; fillColor.g = 200; fillColor.b = 0; // Green fill
    }

    // Start the progress bar animation with specified duration (in milliseconds)
    void startAnimation( Uint32 animDuration ) {
        startTime = SDL_GetTicks();
        duration = animDuration;
        progress = 0.0f;
        isActive = true;
    }

    // Update progress based on elapsed time
    void update() {
        if (!isActive) return;

        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsed = currentTime - startTime;

        if (elapsed >= duration) {
            progress = 1.0f;
            isActive = false;
        }
        else {
            progress = static_cast<float>(elapsed) / duration;
        }
    }

    // Draw the progress bar at specified x, y
    void draw( int x, int y ) {
        if (!isActive) return;

        // Update position
        barRect.x = x;
        barRect.y = y;

        // Draw background
        SDL_FillRect( screen, &barRect, SDL_MapRGB( screen->format, bgColor.r, bgColor.g, bgColor.b ) );

        // Draw border
        SDL_Rect borderRect = { barRect.x, barRect.y, barRect.w, barRect.h };
        SDL_FillRect( screen, &borderRect, SDL_MapRGB( screen->format, 0, 0, 0 ) );

        // Draw inner fill
        SDL_Rect fillRect = {
            barRect.x + borderWidth,
            barRect.y + borderWidth,
            static_cast<Uint16>((barRect.w - 2 * borderWidth) * progress),
            barRect.h - 2 * borderWidth
        };
        SDL_FillRect( screen, &fillRect, SDL_MapRGB( screen->format, fillColor.r, fillColor.g, fillColor.b ) );
    }

    // Check if animation is finished
    bool isFinished() const {
        return !isActive;
    }

    // Optional: Set custom colors
    void setColors( SDL_Color background, SDL_Color fill ) {
        bgColor = background;
        fillColor = fill;
    }
};