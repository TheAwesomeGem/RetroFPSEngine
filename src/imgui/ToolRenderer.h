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
    struct ToolEvent
    {
        enum class Type
        {
            AddActor,
            DeleteActor,
            AddComponent,
            DeleteComponent,
            Unknown
        };

        Type type = Type::Unknown;

        struct ActorData
        {
            Scene::ActorHandle handle;
        };

        struct ActorComponentData
        {
            Scene::ActorHandle handle;
            Scene::ComponentType component;
        };

        union
        {
            ActorData actor_data;
            ActorComponentData actor_component_data;
        };

        static ToolEvent add_actor()
        {
            ToolEvent e = {};
            e.type = Type::AddActor;
            return e;
        }

        static ToolEvent delete_actor(Scene::ActorHandle handle)
        {
            ToolEvent e = {};
            e.type = Type::DeleteActor;
            e.actor_data = ActorData{ handle };
            return e;
        }

        static ToolEvent add_component(Scene::ActorHandle handle, Scene::ComponentType component)
        {
            ToolEvent e = {};
            e.type = Type::AddComponent;
            e.actor_component_data = ActorComponentData{ handle, component };
            return e;
        }

        static ToolEvent delete_component(Scene::ActorHandle handle, Scene::ComponentType component)
        {
            ToolEvent e = {};
            e.type = Type::DeleteComponent;
            e.actor_component_data = ActorComponentData{ handle, component };
            return e;
        }
    };

    struct ToolState
    {
        NOT_COPYABLE_AND_MOVEABLE(ToolState);
        ToolState() = default;

        ImguiResource imgui = ImguiResource{};
        Scene::ActorHandle selected_actor_handle = Scene::ActorHandle{};
        std::vector<ToolEvent> pending_events = {};
    };

    bool create(ToolState& tool, SDL_Window* window, ID3D11Device2* device, ID3D11DeviceContext2* device_context);
    void on_event(const SDL_Event* event);
    void update();
    void render();
    void show_actor_properties(ToolState& tool, Scene::SceneState& scene);
    void show_scene_heirarchy(ToolState& tool, const Scene::SceneState& scene);
    void show_scene_entry(ToolState& tool, const Scene::SceneState& scene, const Scene::Actor& actor);
}
