#include "SDL.h"
#include "SDL_ttf.h"
#include <vector>
#include <string>

#include "Globals.h"

const int FONT_SIZE = 16;
const int SCROLL_SPEED = 16;
const int LINES_VISIBLE = 5;

class TextBox {
public:
    TextBox( int x, int y, int width, int height, const std::string& fontPath, int fontSize );
    ~TextBox();
    void handleEvent( SDL_Event& event );
    void render( SDL_Surface* screen );

    void addLine( const std::string& line );
    std::vector<std::string> wrapText( const std::string& text );
    bool isMouseOver( int mouseX, int mouseY );

private:
    int x, y, width, height;
    int scrollIndex;
    TTF_Font* font;
    SDL_Color textColor;
    std::vector<std::string> lines;

    bool loadFont( const std::string& fontPath, int fontSize );
    SDL_Surface* renderText( const std::string& text );
};