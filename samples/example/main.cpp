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

static ui::Window* rootWindow = nullptr;

static bool assertWindow()
{
    if (rootWindow == nullptr) {
        SDL_Log("There is no root window defined.");
        return false;
    }
    return true;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
    auto* window = new ExampleApp();
    window->setCurrentExemple(0);
    rootWindow = window;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* sdlEvent)
{
    if (!assertWindow()) { return SDL_APP_FAILURE; }

    ui::Event event;
    event.sdlEvent = *sdlEvent;

    if (event.sdlEvent.type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    rootWindow->eventHandler(event);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    if (!assertWindow()) { return SDL_APP_FAILURE; }

    const uint64_t startTime = SDL_GetTicks();
    rootWindow->update();
    BLContext context;
    rootWindow->render(context, {0, 0});

    const uint64_t elapsedTime = SDL_GetTicks() - startTime;
    if (elapsedTime < FRAME_DURATION_MS) {
        SDL_Delay(FRAME_DURATION_MS - elapsedTime);
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
}
