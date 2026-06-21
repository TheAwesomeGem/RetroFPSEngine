//
// Created by TheAwesomeGem on 6/20/2026.
//

#pragma once
#include "src/Common.h"


struct SDLSystemResource
{
    NOT_COPYABLE_AND_MOVEABLE(SDLSystemResource);
    SDLSystemResource() = default;
    ~SDLSystemResource();

    bool create();
    void release();
};
