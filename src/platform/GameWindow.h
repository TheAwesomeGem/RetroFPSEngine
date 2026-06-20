#pragma once

#include "../Common.h"

#include <functional>
#include <SDL3/SDL.h>

#include "SDLWindowWrapper.h"

namespace GameWindow
{
    using SizeCallback = void(*)(Vec2I);
    using EventCallback = void(*)(const SDL_Event* event);

    struct WindowState
    {
        NOT_COPYABLE_AND_MOVEABLE(WindowState);
        WindowState() = default;

        HWND hwnd = nullptr;
        bool should_close = false;
        SDLWindowWrapper resource = SDLWindowWrapper{};
        SizeCallback size_callback = nullptr;
        EventCallback event_callback = nullptr;
    };

    bool create(WindowState* window, Vec2I size);
    void loop(WindowState* window);
    void release(WindowState* window);
}
