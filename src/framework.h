#pragma once

#include <SDL3/SDL_events.h>

#include "window.h"

constexpr int TARGET_FPS = 60; // Target frames per second
constexpr int FRAME_DURATION_MS = 1000 / TARGET_FPS; // Frame duration in milliseconds

namespace ui
{

/**
 * Creates the main application loop that runs the Application.
 * @param rootWindow
 * @return
 */
int runApplication(ui::Window& rootWindow)
{
    bool running = true;
    while (running) {

        uint64_t startTime = SDL_GetTicks(); // Get the current time in milliseconds

        ui::Event event;
        while (SDL_PollEvent(&event.sdlEvent)) {
            // SDL_Log("Event type: %d", event.sdlEvent.type);

            bool isQuit = event.sdlEvent.type == SDL_EVENT_QUIT;
            // std::cout << isQuit << std::endl;
            if (event.sdlEvent.type == SDL_EVENT_QUIT) {
                running = false; // Exit the loop on quit event
            }

            rootWindow.eventHandler(event); // Dispatch event to the widget
        }
        rootWindow.update();
        rootWindow.render(nullptr, {0, 0}); // Render the root window

        // SDL_Renderer* renderer = rootWindow.getRenderer();

        // SDL_SetRenderTarget(renderer, nullptr);
        // SDL_RenderTexture(renderer, rootWindow._texture, nullptr, nullptr);
        // SDL_RenderPresent(renderer); // Present the rendered crendererontent

        uint64_t elapsedTime = SDL_GetTicks() - startTime; // Calculate elapsed time
        if (elapsedTime < FRAME_DURATION_MS) {
            SDL_Delay(FRAME_DURATION_MS - elapsedTime); // Delay to maintain target FPS
        }
    }
    
    // Cleanup
    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0; // Success
}

} // namespace ui
