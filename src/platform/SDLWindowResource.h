//
// Created by TheAwesomeGem on 6/21/2026.
//

#pragma once
#include "src/Common.h"
#include <SDL3/SDL_video.h>

struct SDLWindowResource
{
    NOT_COPYABLE_AND_MOVEABLE(SDLWindowResource);
    SDLWindowResource() = default;
    ~SDLWindowResource();

    bool create(const char *title, const int width, const int height, const SDL_WindowFlags flags);
    void release();

    SDL_Window* wnd = nullptr;
};
