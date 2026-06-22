#include "GameWindow.h"

#include "../Common.h"
#include "../Log.h"

static constexpr const char* LOG_CAT = "GameWindow";

bool GameWindow::create(WindowState& window, const Vec2I size)
{
    release(window);

    if (!window.sdl.create())
    {
        Log::fatal(LOG_CAT, "Failed to initialize SDL.");
        return false;
    }

    SDL_SetHint(SDL_HINT_WINDOWS_GAMEINPUT, "1");

    if (!window.window.create("Game Machine", size.x, size.y, SDL_WINDOW_RESIZABLE))
    {
        Log::fatal(LOG_CAT, "Failed to create SDL Window.");
        return false;
    }

    const SDL_PropertiesID h_props = SDL_GetWindowProperties(window.window.wnd);
    window.hwnd = static_cast<HWND>(SDL_GetPointerProperty(h_props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));

    if (window.hwnd == nullptr)
    {
        Log::fatal(LOG_CAT, "SDL Failed getting platform handle: {}", SDL_GetError());
        return false;
    }

    return true;
}

void GameWindow::loop(WindowState& window, void* callback_receiver)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (window.event_callback)
        {
            window.event_callback(&event);
        }

        switch (event.type)
        {
            case SDL_EVENT_QUIT:
            {
                window.should_close = true;
                break;
            }

            case SDL_EVENT_WINDOW_RESIZED:
            {
                const int width = event.window.data1;
                const int height = event.window.data2;
                if (window.size_callback)
                {
                    window.size_callback(callback_receiver,  Vec2I{ .x = width, .y = height });
                }
                break;
            }
        }
    }
}

void GameWindow::release(WindowState& window)
{
    Log::info(LOG_CAT, "Releasing GameWindow.");

    window.hwnd = nullptr;
    window.sdl.release();
    window.window.release();
}
