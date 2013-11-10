#include "global.h"
#include "drawing.h"
#include "interface.h"

#include <cstdarg>
#include <math.h>

Surface::Surface()
{
}

SDL_Surface* Surface::Load(const char* File)
{
    SDL_Surface* Surf_Temp = NULL;
    SDL_Surface* Surf_Return = NULL;

    //if((Surf_Temp = IMG_Load(File)) == NULL)
        return NULL;

    Surf_Return = SDL_DisplayFormat(Surf_Temp);
    SDL_FreeSurface(Surf_Temp);

    return Surf_Return;
}

bool Surface::Draw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int W, int H)
{
    if(Surf_Dest == NULL || Surf_Src == NULL)
        return false;

    SDL_Rect DestR;

    DestR.x = X;
    DestR.y = Y;

    if(W > 0 && H > 0)
    {
        SDL_Rect SrcR;

        SrcR.x = 0;
        SrcR.y = 0;
        SrcR.w = W;
        SrcR.h = H;

        SDL_BlitSurface(Surf_Src, &SrcR, Surf_Dest, &DestR);
    }
    else
        SDL_BlitSurface(Surf_Src, NULL, Surf_Dest, &DestR);

    return true;
}

bool Surface::DrawFont(SDL_Surface* Surf_Dest, int X, int Y, TTF_Font* font, const char* format, ...)
{
    if(Surf_Dest == NULL)
        return false;

    va_list argList;
    va_start(argList,format);
    char buf[512];
    vsnprintf(buf,512,format,argList);
    va_end(argList);

    SDL_Color foregroundColor = sApp.DefColor;

    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, buf, foregroundColor);

    SDL_Rect DestR;

    DestR.x = X;
    DestR.y = Y;

    SDL_BlitSurface(textSurface, NULL, Surf_Dest, &DestR);

    return true;
}

bool Surface::DrawFontRGB(SDL_Surface* Surf_Dest, int X, int Y, TTF_Font* font, SDL_Color col, const char* format, ...)
{
    if(Surf_Dest == NULL)
        return false;

    va_list argList;
    va_start(argList,format);
    char buf[512];
    vsnprintf(buf,512,format,argList);
    va_end(argList);

    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, buf, col);

    SDL_Rect DestR;

    DestR.x = X;
    DestR.y = Y;

    SDL_BlitSurface(textSurface, NULL, Surf_Dest, &DestR);

    return true;
}

bool Surface::DrawRect(SDL_Surface* Surf_Dest, int X, int Y, int W, int H, Uint32 color)
{
    if(Surf_Dest == NULL)
        return false;

    SDL_Rect DestR;
    DestR.x=X;
    DestR.y=Y;
    DestR.w=W;
    DestR.h=H;
    SDL_FillRect(Surf_Dest,&DestR,color);

    return true;
}
