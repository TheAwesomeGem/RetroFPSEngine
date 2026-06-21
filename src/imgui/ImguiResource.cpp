//
// Created by TheAwesomeGem on 6/21/2026.
//

#include "ImguiResource.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_sdl3.h"
#include <d3d11_2.h>
#include <SDL3/SDL.h>

ImguiResource::~ImguiResource()
{
    release();
}

bool ImguiResource::create(SDL_Window* window, ID3D11Device2* device, ID3D11DeviceContext2* context)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForD3D(window);
    ImGui_ImplDX11_Init(device, context);

    return true;
}

void ImguiResource::release()
{
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplDX11_Shutdown();
    ImGui::DestroyContext();
}
