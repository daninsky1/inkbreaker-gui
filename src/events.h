#pragma once

#include <iostream>

#include <SDL3/SDL_events.h>

#include "layout.h"

namespace ui
{
struct Event {
    SDL_Event sdlEvent; // Type of the event (e.g., SDL_EVENT_QUIT, SDL_EVENT_KEYDOWN, etc.)
    bool handled = false; // Indicates if the event has been handled
    bool hasPosition = false; // True when `position` contains pointer coordinates.
    Position position = {0, 0}; // Local position for the widget currently handling the event.
};

// void eventDispatcher(Event& event)
// {
//     // This function would typically handle the event, e.g., by passing it to the widget's event handler
//     // For this example, we will just print a message
//     std::cout << "Event dispatched." << std::endl;
    
//     // Here you would call the widget's event handler
//     // widget.eventHandler(event);
// }
}
