//
// Created by TheAwesomeGem on 12/20/2025.
//

#pragma once

#include "ImguiResource.h"
#include "imgui_impl_sdl3.h"
#include "../Common.h"
#include "src/scene/Scene.h"

namespace ToolRenderer
{
    using AddActorCallback = void(*)();
    using DeleteActorCallback = void(*)(ActorHandle handle);
    using AddComponentCallback = void(*)(ActorHandle handle, ComponentType component);
    using DeleteComponentCallback = void(*)(ActorHandle handle, ComponentType component);

    struct ToolState
    {
        NOT_COPYABLE_AND_MOVEABLE(ToolState);
        ToolState() = default;

        ImguiResource imgui = ImguiResource{};
        ActorHandle selected_actor_handle = ActorHandle {};
        AddActorCallback add_actor_callback = nullptr;
        DeleteActorCallback delete_actor_callback = nullptr;
        AddComponentCallback add_component_callback = nullptr;
        DeleteComponentCallback delete_component_callback = nullptr;
    };

    bool create(ToolState& tool, SDL_Window* window, ID3D11Device2* device, ID3D11DeviceContext2* device_context);
    void on_event(const SDL_Event* event);
    void update();
    void render();
    void show_actor_properties(const ToolState& tool, Scene& scene);
    void show_scene_heirarchy(ToolState& tool, const Scene& scene);
    void show_scene_entry(ToolState& tool, const Scene& scene, const Actor& actor);
}