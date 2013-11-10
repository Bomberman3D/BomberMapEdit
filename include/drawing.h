#ifndef DRAWING_H
#define DRAWING_H

class Surface
{
public:
    Surface();

    static SDL_Surface* Load(const char* File);
    static bool Draw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int W = 0, int H = 0);
    static bool DrawFont(SDL_Surface* Surf_Dest, int X, int Y, TTF_Font* font, const char* format, ...);
    static bool DrawFontRGB(SDL_Surface* Surf_Dest, int X, int Y, TTF_Font* font, SDL_Color col, const char* format, ...);
    static bool DrawRect(SDL_Surface* Surf_Dest, int X, int Y, int W, int H, Uint32 color);
};

#endif
