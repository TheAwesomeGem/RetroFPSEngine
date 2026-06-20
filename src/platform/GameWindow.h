#pragma once

#include "../Common.h"

#include <functional>
#include <SDL3/SDL.h>

class GameWindow
{
public:
    using SizeCallback = std::function<void(Vec2I)>;
    using EventCallback = std::function<void(const SDL_Event* event)>;

    NOT_COPYABLE_AND_MOVEABLE(GameWindow);

    GameWindow();
    ~GameWindow();

    bool create(Vec2I size);

    void loop();

    [[nodiscard]] HWND get_hwnd() const
    {
        return m_window_handle;
    }

    [[nodiscard]] bool should_close() const
    {
        return m_should_close;
    }

    SDL_Window* m_window_raw = nullptr;
    SizeCallback m_size_callback = nullptr;
    EventCallback m_event_callback = nullptr;

private:
    void release() const;

    HWND m_window_handle = nullptr;
    bool m_should_close = false;
};
