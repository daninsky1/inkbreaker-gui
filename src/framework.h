#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

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
int runApplication(ui::Window& rootWindow);

void setRootWindow(ui::Window* rootWindow);
ui::Window* getRootWindow();
bool hasRootWindow();

SDL_AppResult handleAppEvent(SDL_Event* sdlEvent);
SDL_AppResult handleAppIterate();
void handleAppQuit(SDL_AppResult result);

} // namespace ui
