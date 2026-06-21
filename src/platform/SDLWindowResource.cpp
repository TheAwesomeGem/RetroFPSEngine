//
// Created by TheAwesomeGem on 6/21/2026.
//

#include "SDLWindowResource.h"

SDLWindowResource::~SDLWindowResource()
{
    release();
}

bool SDLWindowResource::create(const char *title, const int width, const int height, const SDL_WindowFlags flags)
{
    wnd = SDL_CreateWindow(title, width, height, flags);

    return wnd != nullptr;
}

void SDLWindowResource::release()
{
    if (wnd)
    {
        SDL_DestroyWindow(wnd);
    }
}
