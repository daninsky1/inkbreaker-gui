// The main reason to use SDL callbacks is to avoid the annoying bug on some
// platforms like Windows that blocks the main thread when dragging the window.
// issues:
// https://github.com/libsdl-org/SDL/issues/9102
// https://github.com/libsdl-org/SDL/issues/1059
// solution:
// https://wiki.libsdl.org/SDL3/README-main-functions
#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "examples.h"
#include "framework.h"
#include "window.h"

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
    auto* window = new ExampleApp();
    window->setCurrentExemple(0);
    ui::setRootWindow(window);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* sdlEvent)
{
    return ui::handleAppEvent(sdlEvent);
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    return ui::handleAppIterate();
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    ui::handleAppQuit(result);
}
