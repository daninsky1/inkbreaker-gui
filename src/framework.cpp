#include "framework.h"

#include <SDL3/SDL.h>
#include <memory>

#include "sdl_render_host.h"

namespace ui {
namespace {

Window* rootWindow = nullptr;
std::unique_ptr<SdlRenderHost> renderHost;

bool assertRootWindow()
{
    if (rootWindow == nullptr) {
        SDL_Log("There is no root window defined.");
        return false;
    }
    return true;
}

bool assertRenderHost()
{
    if (renderHost == nullptr) {
        SDL_Log("There is no render host defined.");
        return false;
    }
    return true;
}

void updateAndRender(Window& window, SdlRenderHost& host, uint64_t startTime)
{
    window.layout(BoxConstraints::tight(window.getSize()));
    host.renderFrame(window);

    const uint64_t elapsedTime = SDL_GetTicks() - startTime;
    if (elapsedTime < FRAME_DURATION_MS) {
        SDL_Delay(FRAME_DURATION_MS - elapsedTime);
    }
}

} // namespace

int runApplication(Window& rootWindow)
{
    {
        SdlRenderHost host(rootWindow);
        bool running = true;
        while (running) {
            const uint64_t startTime = SDL_GetTicks();

            SDL_Event sdlEvent;
            while (SDL_PollEvent(&sdlEvent)) {
                Event event;
                event.sdlEvent = sdlEvent;
                if (event.sdlEvent.type == SDL_EVENT_QUIT) {
                    running = false;
                }

                rootWindow.eventHandler(event);
                host.resize(rootWindow.getSize());
            }

            updateAndRender(rootWindow, host, startTime);
        }
    }

    SDL_Quit();
    return 0;
}

void setRootWindow(Window* window)
{
    rootWindow = window;
    renderHost.reset();
    if (rootWindow != nullptr) {
        renderHost = std::make_unique<SdlRenderHost>(*rootWindow);
    }
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
    if (!assertRenderHost()) {
        return SDL_APP_FAILURE;
    }

    Event event;
    event.sdlEvent = *sdlEvent;

    if (event.sdlEvent.type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    rootWindow->eventHandler(event);
    renderHost->resize(rootWindow->getSize());
    return SDL_APP_CONTINUE;
}

SDL_AppResult handleAppIterate()
{
    if (!assertRootWindow()) {
        return SDL_APP_FAILURE;
    }
    if (!assertRenderHost()) {
        return SDL_APP_FAILURE;
    }

    updateAndRender(*rootWindow, *renderHost, SDL_GetTicks());
    return SDL_APP_CONTINUE;
}

void handleAppQuit(SDL_AppResult result)
{
    (void)result;

    renderHost.reset();
    delete rootWindow;
    rootWindow = nullptr;
    SDL_Quit();
}

} // namespace ui
