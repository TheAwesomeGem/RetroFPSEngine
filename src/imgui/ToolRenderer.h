//
// Created by TheAwesomeGem on 12/20/2025.
//

#pragma once

#include "imgui_impl_sdl3.h"
#include "../Common.h"
#include "src/scene/Scene.h"


struct SDL_Window;
struct ID3D11Device2;
struct ID3D11DeviceContext2;

class ToolRenderer
{
public:
    using AddActorCallback = std::function<void()>;
    using DeleteActorCallback = std::function<void(ActorHandle handle)>;
    using AddComponentCallback = std::function<void(ActorHandle handle, ComponentType component)>;
    using DeleteComponentCallback = std::function<void(ActorHandle handle, ComponentType component)>;

    NOT_COPYABLE_AND_MOVEABLE(ToolRenderer);

    ToolRenderer();
    ~ToolRenderer();

    bool init(SDL_Window* window, ID3D11Device2* device, ID3D11DeviceContext2* device_context);
    void on_event(const SDL_Event* event);
    void update();
    void render();
    void show_actor_properties(Scene& scene);
    void show_scene_heirarchy(const Scene& scene);
    void show_scene_entry(const Scene& scene, const Actor& actor);

    AddActorCallback m_add_actor_callback;
    DeleteActorCallback m_delete_actor_callback;
    AddComponentCallback m_add_component_callback;
    DeleteComponentCallback m_delete_component_callback;

private:
    void show_input_box(const char* label, std::string& value);
    void show_readonly_input_box(const char* label, const std::string& value);

    ActorHandle m_selected_actor_handle;
};
