//
// Created by TheAwesomeGem on 6/20/2026.
//

#pragma once
#include <optional>
#include <SDL3/SDL_video.h>

#include "src/Common.h"


class SDLWindowWrapper final
{
public:
    NOT_COPYABLE_AND_MOVEABLE(SDLWindowWrapper);

    SDLWindowWrapper() = default;
    ~SDLWindowWrapper();

    std::optional<HWND> create_window(const char *title, const int width, const int height, const SDL_WindowFlags flags);
    void release();

    SDL_Window* m_window_raw = nullptr;
};
