#pragma once

#include <deque>

#include "../Common.h"
#include "SDLSystemResource.h"
#include "SDLWindowResource.h"
#include <SDL3/SDL.h>

namespace GameWindow
{
    struct WindowEvent
    {
        enum class Type
        {
            Event,
            Size,
            Unknown
        };

        Type type = Type::Unknown;
        union
        {
            SDL_Event event;
            Vec2I size;
        };

        static WindowEvent sdl_event(const SDL_Event& event)
        {
            WindowEvent e = {};
            e.type = Type::Event;
            e.event = event;
            return e;
        }

        static WindowEvent resized(Vec2I new_size)
        {
            WindowEvent e = {};
            e.type = Type::Size;
            e.size = new_size;
            return e;
        }
    };

    struct WindowState
    {
        NOT_COPYABLE_AND_MOVEABLE(WindowState);
        WindowState() = default;

        HWND hwnd = nullptr;
        bool should_close = false;
        SDLSystemResource sdl = SDLSystemResource{};
        SDLWindowResource window = SDLWindowResource{};
    };

    bool create(WindowState& window, Vec2I size);
    void loop(WindowState& window, std::vector<WindowEvent>& events);
    void release(WindowState& window);
}
