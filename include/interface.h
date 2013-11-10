#ifndef INTERFACE_H
#define INTERFACE_H

#include "global.h"
#include <vector>

class Application
{
    public:
        Application();
        bool Init();
        int Run();
        void OnEvent(SDL_Event* Event);

        SDL_Surface* MainSurface;

        SDL_Color DefColor;
        void SetDefColor(const uint8 r,const uint8 g,const uint8 b) { DefColor.r = r; DefColor.g = g; DefColor.b = b; }

    private:
        bool Running;
};

static Application sApp;

#define MAX_KEYS 1024

enum SelectType
{
    SELECT_TYPE_TYPE             = 1,
    SELECT_TYPE_TEXTURE          = 2,
    SELECT_TYPE_MAPSIZE          = 3,
    SELECT_TYPE_TYPE_MULTIPLE    = 4,
    SELECT_TYPE_TEXTURE_MULTIPLE = 5,
};

struct Map
{
    struct cell
    {
        uint16 type;
        uint16 texture;
    };
    // Field dynamic storage
    std::vector<std::vector<cell>> field;
    // Skybox ID (will be implemented later)
    uint16 skybox;
    // Map name
    std::string mapname;
};

class Interface
{
    public:
        Interface();
        ~Interface();

        void Render();
        void MouseClick(uint16 x, uint16 y, bool left);
        void KeyDown(uint16 key);
        void KeyUp(uint16 key);
        bool IsKeyPressed(uint16 key) { if (key > MAX_KEYS) return false; return keys[key]; }

        SDL_Surface* MainSurface;

        Uint32 MapRGB(const uint8 r, const uint8 g, const uint8 b) { return SDL_MapRGB(MainSurface->format,r,g,b); }
        SDL_Color MapSDLRGB(const uint8 r, const uint8 g, const uint8 b) { SDL_Color col; col.r = r; col.g = g; col.b = b; return col; }

        void SaveMap(const char* path);
        bool LoadMap(const char* path);

    private:
        bool DisplayChanged;
        bool keys[MAX_KEYS];
        TTF_Font *font,*smallfont;

        struct
        {
            Map *ActMap;
            bool MapLoaded;
            std::string MapPath;

            bool menu;
            uint16 menu_x;
            uint16 menu_y;
            uint16 cell_x;
            uint16 cell_y;

            uint8 select_type;
            //retains cell_x and cell_y from menu
            uint16 selected;
        } Storage;
};

static Interface sInterface;

#endif
