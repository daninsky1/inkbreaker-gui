#include "framework.h"

#include <SDL3/SDL.h>

namespace ui {
namespace {

Window* rootWindow = nullptr;

bool assertRootWindow()
{
    if (rootWindow == nullptr) {
        SDL_Log("There is no root window defined.");
        return false;
    }
    return true;
}

void updateAndRender(Window& window, uint64_t startTime)
{
    window.update();
    window.render(nullptr, {0, 0});

    const uint64_t elapsedTime = SDL_GetTicks() - startTime;
    if (elapsedTime < FRAME_DURATION_MS) {
        SDL_Delay(FRAME_DURATION_MS - elapsedTime);
    }
}

} // namespace

int runApplication(Window& rootWindow)
{
    bool running = true;
    while (running) {
        const uint64_t startTime = SDL_GetTicks();

        Event event;
        while (SDL_PollEvent(&event.sdlEvent)) {
            if (event.sdlEvent.type == SDL_EVENT_QUIT) {
                running = false;
            }

            rootWindow.eventHandler(event);
        }

        updateAndRender(rootWindow, startTime);
    }

    SDL_Quit();
    return 0;
}

void setRootWindow(Window* window)
{
    rootWindow = window;
}

Window* getRootWindow()
{
    return rootWindow;
}

bool hasRootWindow()
{
    return rootWindow != nullptr;
}

SDL_AppResult handleAppEvent(SDL_Event* sdlEvent)
{
    if (!assertRootWindow()) {
        return SDL_APP_FAILURE;
    }

    Event event;
    event.sdlEvent = *sdlEvent;

    if (event.sdlEvent.type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    rootWindow->eventHandler(event);
    return SDL_APP_CONTINUE;
}

SDL_AppResult handleAppIterate()
{
    if (!assertRootWindow()) {
        return SDL_APP_FAILURE;
    }

    updateAndRender(*rootWindow, SDL_GetTicks());
    return SDL_APP_CONTINUE;
}

void handleAppQuit(SDL_AppResult result)
{
    (void)result;

    delete rootWindow;
    rootWindow = nullptr;
    SDL_Quit();
}

} // namespace ui
