//
// Created by TheAwesomeGem on 6/20/2026.
//

#include "SDLSystemResource.h"

#include <SDL3/SDL_init.h>

SDLSystemResource::~SDLSystemResource()
{
}

bool SDLSystemResource::create()
{
    return SDL_Init(SDL_INIT_VIDEO);
}

void SDLSystemResource::release()
{
    SDL_Quit();
}
