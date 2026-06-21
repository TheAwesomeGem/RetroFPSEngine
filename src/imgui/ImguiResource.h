//
// Created by TheAwesomeGem on 6/21/2026.
//

#pragma once
#include "src/Common.h"

struct SDL_Window;
class ID3D11Device2;
class ID3D11DeviceContext2;

struct ImguiResource
{
    NOT_COPYABLE_AND_MOVEABLE(ImguiResource);
    ImguiResource() = default;
    ~ImguiResource();

    bool create(SDL_Window* window, ID3D11Device2* device, ID3D11DeviceContext2* context);
    void release();
};
