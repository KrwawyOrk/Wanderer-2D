#include "TextBox.h"

#include <sstream>

TextBox::TextBox( int x, int y, int width, int height, const std::string& fontPath, int fontSize )
    : x( x ), y( y ), width( width ), height( height ), scrollIndex( 0 ), font( NULL ), textColor( { 0, 255, 0 } ) {
    loadFont( fontPath, fontSize );
}

TextBox::~TextBox() {
    if (font) {
        TTF_CloseFont( font );
    }
}

bool TextBox::loadFont( const std::string& fontPath, int fontSize ) {
    font = TTF_OpenFont( fontPath.c_str(), fontSize );
    return font != NULL;
}

SDL_Surface* TextBox::renderText( const std::string& text ) {
    return TTF_RenderText_Solid( font, text.c_str(), textColor );
}

bool TextBox::isMouseOver( int mouseX, int mouseY ) {
    return mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height;
}

void TextBox::handleEvent( SDL_Event& event ) {
    int mouseX, mouseY;
    SDL_GetMouseState( &mouseX, &mouseY );
    if (isMouseOver( mouseX, mouseY )) {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_UP) {
                if (scrollIndex > 0) {
                    scrollIndex--;
                }
            }
            else if (event.key.keysym.sym == SDLK_DOWN) {
                if (scrollIndex < lines.size() - LINES_VISIBLE) {
                    scrollIndex++;
                }
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_WHEELUP) {
                if (scrollIndex > 0) {
                    scrollIndex--;
                }
            }
            else if (event.button.button == SDL_BUTTON_WHEELDOWN) {
                if (scrollIndex < lines.size() - LINES_VISIBLE) {
                    scrollIndex++;
                }
            }
        }
    }
}

void TextBox::render( SDL_Surface* screen ) {
    int yPosition = y;
    for (int i = 0; i < LINES_VISIBLE; ++i) {
        int lineIndex = scrollIndex + i;
        if (lineIndex < lines.size()) {
            SDL_Surface* textSurface = renderText( lines[lineIndex] );
            SDL_Rect textLocation = { x, yPosition, 0, 0 };
            SDL_BlitSurface( textSurface, NULL, screen, &textLocation );
            SDL_FreeSurface( textSurface );
            yPosition += FONT_SIZE;
        }
    }
}

std::vector<std::string> TextBox::wrapText( const std::string& text ) {
    std::vector<std::string> wrappedLines;
    std::istringstream stream( text );
    std::string word;
    std::string currentLine;

    while (stream >> word) {
        std::string testLine = currentLine + (currentLine.empty() ? "" : " ") + word;
        int width = 0, height = 0;
        TTF_SizeText( font, testLine.c_str(), &width, &height );
        if (width > this->width) {
            if (!currentLine.empty()) {
                wrappedLines.push_back( currentLine );
            }
            currentLine = word;
        }
        else {
            currentLine = testLine;
        }
    }

    if (!currentLine.empty()) {
        wrappedLines.push_back( currentLine );
    }

    return wrappedLines;
}

void TextBox::addLine( const std::string& line ) {
    std::string prefixedLine = "// " + line;  // Dodanie prefiksu do linii
    std::vector<std::string> wrappedLines = wrapText( prefixedLine );
    lines.insert( lines.end(), wrappedLines.begin(), wrappedLines.end() );
    if (lines.size() > LINES_VISIBLE && scrollIndex < lines.size() - LINES_VISIBLE) {
        scrollIndex = lines.size() - LINES_VISIBLE;
    }
}