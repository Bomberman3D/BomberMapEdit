#include "global.h"
#include "interface.h"

int SDL_main(int argc, char *argv[])
{
    if(!sApp.Init())
        return -1;

    sInterface.MainSurface = sApp.MainSurface;

    return sApp.Run();
}

bool Application::Init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return false;

    if(TTF_Init() < 0)
        return false;

    if((MainSurface = SDL_SetVideoMode(1024+200, 768, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
        return false;

    SDL_EnableUNICODE(1);

    SDL_WM_SetCaption("Bomberman3D - Map Editor",NULL);

    return true;
}

void Application::OnEvent(SDL_Event* Event) 
{
    switch(Event->type)
    {
        case SDL_QUIT:
            Running = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(Event->button.button == SDL_BUTTON_LEFT)
                sInterface.MouseClick(Event->button.x, Event->button.y, true);
            else
                sInterface.MouseClick(Event->button.x, Event->button.y, false);
            break;
        case SDL_KEYDOWN:
            sInterface.KeyDown(Event->key.keysym.unicode);
            break;
        case SDL_KEYUP:
            sInterface.KeyUp(Event->key.keysym.unicode);
            break;
    }
}

Application::Application()
{
    DefColor.r = 255;
    DefColor.g = 255;
    DefColor.b = 255;
}

int Application::Run()
{
    Running = true;

    SDL_Event Event;

    while(Running)
    {
        while(SDL_PollEvent(&Event))
        {
            OnEvent(&Event);
        }

        sInterface.Render();
        SDL_Flip(MainSurface);
    }

    SDL_FreeSurface(MainSurface);
    SDL_Quit();

    return 0;
}
