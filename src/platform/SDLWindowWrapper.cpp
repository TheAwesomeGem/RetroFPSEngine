//
// Created by TheAwesomeGem on 6/20/2026.
//

#include "SDLWindowWrapper.h"

#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_init.h>

SDLWindowWrapper::~SDLWindowWrapper()
{
    release();
}

std::optional<HWND> SDLWindowWrapper::create_window(const char* title, const int width, const int height, const SDL_WindowFlags flags)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        return std::nullopt;
    }

    SDL_SetHint(SDL_HINT_WINDOWS_GAMEINPUT, "1");

    m_window_raw = SDL_CreateWindow(title, width, height, flags);

    if (m_window_raw == nullptr)
    {
        return std::nullopt;
    }

    const SDL_PropertiesID h_props = SDL_GetWindowProperties(m_window_raw);
    HWND hwnd = static_cast<HWND>(SDL_GetPointerProperty(h_props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));

    if (hwnd == nullptr)
    {
        return std::nullopt;
    }

    return hwnd;
}

void SDLWindowWrapper::release()
{
    if (m_window_raw)
    {
        SDL_DestroyWindow(m_window_raw);
    }

    SDL_Quit();
}
