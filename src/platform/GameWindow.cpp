#include "GameWindow.h"

#include "../Common.h"
#include "../Log.h"

static constexpr const char* LOG_CAT = "GameWindow";

bool GameWindow::create(WindowState* window, const Vec2I size)
{
    release(window);

    auto hwnd = window->resource.create_window("Game Machine", size.x, size.y, SDL_WINDOW_RESIZABLE);
    if (!hwnd)
    {
        Log::fatal(LOG_CAT, "Failed to create SDL Window.");
        return false;
    }

    window->hwnd = *hwnd;

    Log::info(LOG_CAT, "Constructed GameWindow.");

    return true;
}

void GameWindow::loop(WindowState* window)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (window->event_callback)
        {
            window->event_callback(&event);
        }

        switch (event.type)
        {
            case SDL_EVENT_QUIT:
            {
                window->should_close = true;
                break;
            }

            case SDL_EVENT_WINDOW_RESIZED:
            {
                const int width = event.window.data1;
                const int height = event.window.data2;
                if (window->size_callback)
                {
                    window->size_callback(Vec2I{ .x = width, .y = height });
                }
                break;
            }
        }
    }
}

void GameWindow::release(WindowState* window)
{
    Log::info(LOG_CAT, "Releasing GameWindow.");

    window->hwnd = nullptr;
    window->resource.release();
}
