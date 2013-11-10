#include "global.h"
#include "interface.h"
#include "drawing.h"

#include <windows.h>
#include <math.h>

Interface::Interface()
{
    for (uint16 i = 0; i < MAX_KEYS; i++)
        keys[i] = false;

    TTF_Init();
    font = TTF_OpenFont("D:\\WINDOWS\\Fonts\\trebuc.TTF", 15);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    smallfont = TTF_OpenFont("D:\\WINDOWS\\Fonts\\trebuc.TTF", 12);
    TTF_SetFontStyle(smallfont, TTF_STYLE_BOLD);

    Storage.ActMap = NULL;
    Storage.MapLoaded = false;
    Storage.menu = false;
    Storage.select_type = 0;

    DisplayChanged = true;
}

Interface::~Interface()
{
}

void Interface::Render()
{
    if (!DisplayChanged)
        return;
    if (!MainSurface)
        return;
    if (!font || !smallfont)
        return;

    DisplayChanged = false;

    // Frame
    Surface::DrawRect(MainSurface,0,0,1024+200,768,MapRGB(255,255,255));
    Surface::DrawRect(MainSurface,5,5,1014+200,758,MapRGB(0,0,0));

    // Working space
    Surface::DrawRect(MainSurface,5,40,1024-2*5,768-40-5,MapRGB(100,100,100));

    // loading and saving
    // Nova Mapa button
    Surface::DrawRect(MainSurface,5,5,150,35,MapRGB(0,0,200));
    Surface::DrawRect(MainSurface,8,8,144,29,MapRGB(0,0,150));
    Surface::DrawFontRGB(MainSurface,16,12,font,MapSDLRGB(255,255,255),"Nova mapa [N]");
    // Nacist Mapu button
    Surface::DrawRect(MainSurface,155,5,150,35,MapRGB(0,0,200));
    Surface::DrawRect(MainSurface,158,8,144,29,MapRGB(0,0,150));
    Surface::DrawFontRGB(MainSurface,166,12,font,MapSDLRGB(255,255,255),"Nacist mapu [L]");
    // Ulozit Mapu button
    Surface::DrawRect(MainSurface,305,5,150,35,MapRGB(0,0,200));
    Surface::DrawRect(MainSurface,308,8,144,29,MapRGB(0,0,150));
    Surface::DrawFontRGB(MainSurface,316,12,font,MapSDLRGB(255,255,255),"Ulozit mapu [S]");

    // editation buttons
    // Zmena velikosti button
    Surface::DrawRect(MainSurface,455,5,150,35,MapRGB(0,200,0));
    Surface::DrawRect(MainSurface,458,8,144,29,MapRGB(0,150,0));
    Surface::DrawFontRGB(MainSurface,466,15,smallfont,MapSDLRGB(255,255,255),"Zmena velikosti [U]");
    // Zmena typu button
    Surface::DrawRect(MainSurface,605,5,150,35,MapRGB(0,200,0));
    Surface::DrawRect(MainSurface,608,8,144,29,MapRGB(0,150,0));
    Surface::DrawFontRGB(MainSurface,616,15,smallfont,MapSDLRGB(255,255,255),"Hr. zmena typu [O]");
    // Zmena textury button
    Surface::DrawRect(MainSurface,755,5,150,35,MapRGB(0,200,0));
    Surface::DrawRect(MainSurface,758,8,144,29,MapRGB(0,150,0));
    Surface::DrawFontRGB(MainSurface,766,15,smallfont,MapSDLRGB(255,255,255),"Hr. zmena text. [P]");

    // Draw map
    if (Storage.ActMap && Storage.MapLoaded)
    {
        Uint32 cellcolor = MapRGB(0,0,0);
        for (uint16 i = 0; i < Storage.ActMap->field.size(); i++)
        {
            for (uint16 j = 0; j < Storage.ActMap->field[0].size(); j++)
            {
                switch (Storage.ActMap->field[i][j].type)
                {
                    case 0:
                        cellcolor = MapRGB(0,190,0);
                        break;
                    case 1:
                        cellcolor = MapRGB(190,0,0);
                        break;
                    case 2:
                        cellcolor = MapRGB(0,0,190);
                        break;
                    case 3:
                        cellcolor = MapRGB(190,190,190);
                        break;
                }

                Surface::DrawRect(MainSurface,2+5+i*(18+2),2+40+j*(18+2),18,18,cellcolor);
                Surface::DrawFont(MainSurface,2+2+5+i*(18+2),2+2+40+j*(18+2),smallfont,"%u",Storage.ActMap->field[i][j].texture);
            }
        }
    }

    // Draw cell click menu
    if (Storage.menu)
    {
        Surface::DrawRect(MainSurface,Storage.menu_x, Storage.menu_y, 150,73,MapRGB(100,0,100));
        Surface::DrawFontRGB(MainSurface,Storage.menu_x+5,Storage.menu_y+4,font,MapSDLRGB(255,255,255),"Zmenit typ");
        Surface::DrawFontRGB(MainSurface,Storage.menu_x+5,Storage.menu_y+4+22*1,font,MapSDLRGB(255,255,255),"Zmenit texturu");
        Surface::DrawFontRGB(MainSurface,Storage.menu_x+5,Storage.menu_y+4+22*2,font,MapSDLRGB(255,255,255),"Nic");
    }

    // Draw cell attributes select
    if (Storage.select_type > 0)
    {
        if (Storage.select_type == SELECT_TYPE_TYPE || Storage.select_type == SELECT_TYPE_TYPE_MULTIPLE)
        {
            Surface::DrawFontRGB(MainSurface,1024+4,40+4,font,MapSDLRGB(255,255,255),"Vyberte typ:");

            Surface::DrawFontRGB(MainSurface,1024+4+10,40+30+22*0,font,MapSDLRGB(200,200,255),"Plocha");
            Surface::DrawFontRGB(MainSurface,1024+4+10,40+30+22*1,font,MapSDLRGB(200,200,255),"Pevna kostka");
            Surface::DrawFontRGB(MainSurface,1024+4+10,40+30+22*2,font,MapSDLRGB(200,200,255),"Bedna");
            Surface::DrawFontRGB(MainSurface,1024+4+10,40+30+22*3,font,MapSDLRGB(200,200,255),"Start. pozice");

            Surface::DrawFont(MainSurface,1024+2,40+30+22*Storage.selected,smallfont,"o");
        }
        else if (Storage.select_type == SELECT_TYPE_TEXTURE || Storage.select_type == SELECT_TYPE_TEXTURE_MULTIPLE)
        {
            Surface::DrawFontRGB(MainSurface,1024+4,40+4,font,MapSDLRGB(255,255,255),"Vyberte texturu:");

            Surface::DrawFont(MainSurface,1024+8,40+30+6,smallfont,"%u",Storage.selected);
            Surface::DrawRect(MainSurface,1024+8+48,40+30+4,18,18,MapRGB(0,200,0));
            Surface::DrawFont(MainSurface,1024+8+52,40+30+4,font,"+");
            Surface::DrawRect(MainSurface,1024+8+48+20,40+30+4,18,18,MapRGB(200,0,0));
            Surface::DrawFont(MainSurface,1024+8+52+20,40+30+4,font,"-");

            //TODO: preview
        }
        else if (Storage.select_type == SELECT_TYPE_MAPSIZE)
        {
            Surface::DrawFontRGB(MainSurface,1024+4,40+4,font,MapSDLRGB(255,255,255),"Zvetseni/zmenseni:");
            Surface::DrawRect(MainSurface,1024+4,40+30+24*0,150,22,MapRGB(0,0,150));
            Surface::DrawFont(MainSurface,1024+8,40+30+4+24*0,smallfont,"Zvetsit >");
            Surface::DrawRect(MainSurface,1024+4,40+30+24*1,150,22,MapRGB(0,0,150));
            Surface::DrawFont(MainSurface,1024+8,40+30+4+24*1,smallfont,"< Zmensit");
            Surface::DrawRect(MainSurface,1024+4,40+30+24*2,150,22,MapRGB(0,0,150));
            Surface::DrawFont(MainSurface,1024+8,40+30+4+24*2,smallfont,"Zvetsit v");
            Surface::DrawRect(MainSurface,1024+4,40+30+24*3,150,22,MapRGB(0,0,150));
            Surface::DrawFont(MainSurface,1024+8,40+30+4+24*3,smallfont,"^ Zmensit");
        }
    }
}

void Interface::KeyDown(uint16 key)
{
    if (key > MAX_KEYS)
        return;

    if (!keys[key])
        keys[key] = true;

    // Events on keypress
    // New map (terrible hack, TODO: fix it)
    if (key == 'n')
        MouseClick(10,10,true);
    if (key == 'u')
        MouseClick(456,6,true);
    if (key == 'o')
        MouseClick(608,6,true);
    if (key == 'p')
        MouseClick(758,6,true);
}

void Interface::KeyUp(uint16 key)
{
    if (key > MAX_KEYS)
        return;

    if (keys[key])
        keys[key] = false;
}

bool InSquare(uint32 srcx, uint32 srcy, uint32 x1, uint32 y1, uint32 x2, uint32 y2)
{
    return ((srcx > x1 && srcx < x2) && (srcy > y1 && srcy < y2));
}

void Interface::MouseClick(uint16 x, uint16 y, bool left)
{
    Map::cell DefaultCell;
    DefaultCell.texture = 0;
    DefaultCell.type = 0;

    // Nova Mapa
    if (InSquare(x,y,5,5,155,40))
    {
        Storage.ActMap = new Map;
        Storage.ActMap->field.resize(10);

        for (int i = 0; i < 10; i++)
        {
            Storage.ActMap->field[i].resize(10);
            for (int j = 0; j < 10; j++)
                Storage.ActMap->field[i][j] = DefaultCell;
        }

        Storage.ActMap->mapname = "New Map";
        Storage.MapPath = "";
        Storage.ActMap->skybox = 0;

        Storage.MapLoaded = true;
    }

    // Nacist Mapu
    if (InSquare(x,y,155,5,305,40))
    {
        OPENFILENAME ofn;
        char filename[MAX_PATH] = "";

        ZeroMemory(&ofn, sizeof(ofn));

        ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
        ofn.lpstrFilter = "Bomberman Maps (*.bm)\0*.bm\0All Files (*.*)\0*.*\0";
        ofn.lpstrFile = filename;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        ofn.lpstrDefExt = "bm";

        if (GetOpenFileName(&ofn))
        {
            Storage.MapPath = filename;
            LoadMap(Storage.MapPath.c_str());
            // TODO: handle fail!
            Storage.MapLoaded = true;
        }
    }
    // Ulozit Mapu
    if (InSquare(x,y,305,5,455,40))
    {
        if (strlen(Storage.MapPath.c_str()) < 2)
        {
            OPENFILENAME ofn;
            char filename[MAX_PATH] = "";

            ZeroMemory(&ofn, sizeof(ofn));

            ofn.lStructSize = sizeof(ofn);
            ofn.lpstrFilter = "Bomberman Maps (*.bm)\0*.bm\0All Files (*.*)\0*.*\0";
            ofn.lpstrFile = filename;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
            ofn.lpstrDefExt = "bm";

            if (GetSaveFileName(&ofn))
                Storage.MapPath = filename;
            else
                return;
        }
        SaveMap(Storage.MapPath.c_str());
    }

    // Zmena velikosti
    if (InSquare(x,y,455,5,605,40))
    {
        Storage.select_type = SELECT_TYPE_MAPSIZE;
    }
    // Hr. zmena typu
    if (InSquare(x,y,605,5,755,40))
    {
        Storage.select_type = SELECT_TYPE_TYPE_MULTIPLE;
        Storage.selected = 0;
    }
    // Hr. zmena textury
    if (InSquare(x,y,755,5,905,40))
    {
        Storage.select_type = SELECT_TYPE_TEXTURE_MULTIPLE;
        Storage.selected = 0;
    }

    if (Storage.select_type == SELECT_TYPE_TEXTURE || Storage.select_type == SELECT_TYPE_TEXTURE_MULTIPLE)
    {
        if (InSquare(x,y,1024+8+48,40+30+4,1024+8+48+18,40+30+4+18))
            Storage.selected += 1;
        else if (InSquare(x,y,1024+8+48+20,40+30+4,1024+8+48+20+18,40+30+4+18))
            if (Storage.selected > 0)
                Storage.selected -= 1;
    }

    // Somewhere in map field
    if (InSquare(x,y,5,40,1019,763))
    {
        if (Storage.menu)
        {
            Storage.menu = false;
            if (InSquare(x,y,Storage.menu_x,Storage.menu_y,Storage.menu_x+150,Storage.menu_y+4+22))
            {
                Storage.select_type = SELECT_TYPE_TYPE;
                Storage.selected = Storage.ActMap->field[Storage.cell_x][Storage.cell_y].type;
            }
            else if (InSquare(x,y,Storage.menu_x+4+22,Storage.menu_y,Storage.menu_x+150,Storage.menu_y+4+22*2))
            {
                Storage.select_type = SELECT_TYPE_TEXTURE;
                Storage.selected = Storage.ActMap->field[Storage.cell_x][Storage.cell_y].texture;
            }
        }
        else if (Storage.select_type == SELECT_TYPE_TYPE_MULTIPLE)
        {
            uint32 cell_x = uint32(floor(float(x-5-2)/(18.0f+2.0f)));
            uint32 cell_y = uint32(floor(float(y-40-2)/(18.0f+2.0f)));

            if (cell_x < Storage.ActMap->field.size() &&
                cell_y < Storage.ActMap->field[0].size())
                Storage.ActMap->field[cell_x][cell_y].type = Storage.selected;
        }
        else if (Storage.select_type == SELECT_TYPE_TEXTURE_MULTIPLE)
        {
            uint32 cell_x = uint32(floor(float(x-5-2)/(18.0f+2.0f)));
            uint32 cell_y = uint32(floor(float(y-40-2)/(18.0f+2.0f)));

            if (cell_x < Storage.ActMap->field.size() &&
                cell_y < Storage.ActMap->field[0].size())
                Storage.ActMap->field[cell_x][cell_y].texture = Storage.selected;
        }
        else if (Storage.ActMap && Storage.MapLoaded)
        {
            uint32 cell_x = uint32(floor(float(x-5-2)/(18.0f+2.0f)));
            uint32 cell_y = uint32(floor(float(y-40-2)/(18.0f+2.0f)));

            Storage.menu = true;
            Storage.menu_x = x;
            Storage.menu_y = y;
            Storage.cell_x = cell_x;
            Storage.cell_y = cell_y;
        }
    }

    // Select in right side
    if (Storage.select_type > 0 && InSquare(x,y,1024+4,40+30,1024+4+200,768) && Storage.MapLoaded)
    {
        if (Storage.select_type == SELECT_TYPE_MAPSIZE)
        {
            if (InSquare(x,y,1024+4,40+30+24*0,1024+4+150,40+30+24*0+22))
            {
                Storage.ActMap->field.resize(Storage.ActMap->field.size()+1);
                Storage.ActMap->field[Storage.ActMap->field.size()-1].resize(Storage.ActMap->field[0].size());
            }
            else if (InSquare(x,y,1024+4,40+30+24*1,1024+4+150,40+30+24*1+22))
            {
                if (Storage.ActMap->field.size() > 1)
                {
                    Storage.ActMap->field[Storage.ActMap->field.size()-1].clear();
                    Storage.ActMap->field.resize(Storage.ActMap->field.size()-1);
                }
            }
            else if (InSquare(x,y,1024+4,40+30+24*2,1024+4+150,40+30+24*2+22))
            {
                for (uint16 i = 0; i < Storage.ActMap->field.size(); i++)
                {
                    Storage.ActMap->field[i].resize(Storage.ActMap->field[i].size()+1);
                    Storage.ActMap->field[i][Storage.ActMap->field[i].size()-1] = DefaultCell;
                }
            }
            else if (InSquare(x,y,1024+4,40+30+24*3,1024+4+150,40+30+24*3+22))
            {
                if (Storage.ActMap->field[0].size() > 1)
                {
                    for (uint16 i = 0; i < Storage.ActMap->field.size(); i++)
                        Storage.ActMap->field[i].resize(Storage.ActMap->field[i].size()-1);
                }
            }
        }
        else if (Storage.select_type == SELECT_TYPE_TYPE)
        {
            for (int i = 0; i < ((768-(40+30))/22); i++)
            {
                if (InSquare(x,y,1024+4,40+30+22*i,1024+4+200,40+30+22*(i+1)))
                {
                    Storage.ActMap->field[Storage.cell_x][Storage.cell_y].type = i;
                    Storage.selected = i;
                    break;
                }
            }
        }
        else if (Storage.select_type == SELECT_TYPE_TYPE_MULTIPLE)
        {
            for (int i = 0; i < ((768-(40+30))/22); i++)
            {
                if (InSquare(x,y,1024+4,40+30+22*i,1024+4+200,40+30+22*(i+1)))
                {
                    Storage.selected = i;
                    break;
                }
            }
        }
    }

    DisplayChanged = true;
}

/////////////////
// file elements

/*
header
    namesize   32bit
    name       string
    skybox     16bit

data
    x          32bit
    y          32bit
    type       16bit
    texture    16bit
*/

void Interface::SaveMap(const char *path)
{
    FILE* pFile = fopen(path,"wb");
    if (!pFile)
        return;

    uint32 namesize = strlen(Storage.ActMap->mapname.c_str());
    fwrite(&namesize,4,1,pFile);
    fwrite(Storage.ActMap->mapname.c_str(),1,strlen(Storage.ActMap->mapname.c_str()),pFile);
    fwrite(&Storage.ActMap->skybox,2,1,pFile);

    for (uint32 i = 0; i < Storage.ActMap->field.size(); i++)
    {
        for (uint32 j = 0; j < Storage.ActMap->field[0].size(); j++)
        {
            fwrite(&i,4,1,pFile);
            fwrite(&j,4,1,pFile);
            fwrite(&Storage.ActMap->field[i][j].type,2,1,pFile);
            fwrite(&Storage.ActMap->field[i][j].texture,2,1,pFile);
        }
    }

    fclose(pFile);
}

struct Chunk
{
    uint32 x;
    uint32 y;
    uint16 type;
    uint16 texture;
};

bool Interface::LoadMap(const char *path)
{
    FILE *pFile = fopen(path,"r");
    if (!pFile)
        return false;

    if (!Storage.ActMap)
        Storage.ActMap = new Map;

    uint32 namesize = 0;
    fread(&namesize,4,1,pFile);
    fread(&Storage.ActMap->mapname,1,namesize,pFile);
    fread(&Storage.ActMap->skybox,2,1,pFile);

    Chunk* pChunk = new Chunk;

    //field 1x1, will be resized
    Storage.ActMap->field.resize(1);
    Storage.ActMap->field[0].resize(1);

    while(fread(pChunk,sizeof(Chunk),1,pFile) > 0)
    {
        if (pChunk->x > Storage.ActMap->field.size()-1)
        {
            Storage.ActMap->field.resize(pChunk->x+1);
            Storage.ActMap->field[Storage.ActMap->field.size()-1].resize(Storage.ActMap->field[0].size());
        }
        if (pChunk->y > Storage.ActMap->field[0].size()-1)
        {
            for (uint32 i = 0; i < Storage.ActMap->field.size(); i++)
                Storage.ActMap->field[i].resize(pChunk->y+1);
        }
        Storage.ActMap->field[pChunk->x][pChunk->y].type = pChunk->type;
        Storage.ActMap->field[pChunk->x][pChunk->y].texture = pChunk->texture;
    }

    return true;
}
