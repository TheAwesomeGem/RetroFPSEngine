#include "GameWindow.h"

#include "../Common.h"
#include "../Log.h"

static constexpr const char* LOG_CAT = "GameWindow";

GameWindow::GameWindow()
{
    Log::info(LOG_CAT, "Constructed.");
}

GameWindow::~GameWindow()
{
    release();
}

bool GameWindow::create(const Vec2I size)
{
    if (m_window_raw != nullptr)
    {
        release();
    }

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        Log::fatal(LOG_CAT, "Failed to initialize SDL.");
        return false;
    }

    SDL_SetHint(SDL_HINT_WINDOWS_GAMEINPUT, "1");
    m_window_raw = SDL_CreateWindow("Game Machine", size.x, size.y, SDL_WINDOW_RESIZABLE);

    if (m_window_raw == nullptr)
    {
        Log::fatal(LOG_CAT, "Failed to create SDL Window.");
        return false;
    }

    const SDL_PropertiesID h_props = SDL_GetWindowProperties(m_window_raw);
    m_window_handle = static_cast<HWND>(SDL_GetPointerProperty(h_props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));

    if (m_window_handle == nullptr)
    {
        Log::fatal(LOG_CAT, "SDL Failed getting platform handle: {}", SDL_GetError());
        return false;
    }

    return true;
}

void GameWindow::loop()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        m_event_callback(&event);

        switch (event.type)
        {
            case SDL_EVENT_QUIT:
            {
                m_should_close = true;
                break;
            }

            case SDL_EVENT_WINDOW_RESIZED:
            {
                const int width = event.window.data1;
                const int height = event.window.data2;
                if (m_size_callback)
                {
                    m_size_callback(Vec2I{ .x = width, .y = height });
                }
                break;
            }
        }
    }
}

void GameWindow::release() const
{
    Log::info(LOG_CAT, "Releasing the platform.");

    if (m_window_raw != nullptr)
    {
        SDL_DestroyWindow(m_window_raw);
    }

    SDL_Quit();
}
