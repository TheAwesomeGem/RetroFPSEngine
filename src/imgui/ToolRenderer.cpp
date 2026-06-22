//
// Created by TheAwesomeGem on 12/20/2025.
//

#include "ToolRenderer.h"

#include <d3d11_2.h>

#include "imgui.h"
#include "ImguiResource.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_sdl3.h"

static std::string uuid_to_string(const uuids::uuid& id)
{
    std::span<std::byte const> byte_span = id.as_bytes();

    return std::format(
        "{:08x}-{:04x}-{:04x}-{:04x}-{:012x}",
        *reinterpret_cast<const uint32_t*>(&byte_span[0]),
        *reinterpret_cast<const uint16_t*>(&byte_span[4]),
        *reinterpret_cast<const uint16_t*>(&byte_span[6]),
        *reinterpret_cast<const uint16_t*>(&byte_span[8]),
        *reinterpret_cast<const uint64_t*>(&byte_span[10])
    );
}

static const char* get_shader_type_str(ShaderHolder::ShaderRenderType shader_type)
{
    switch (shader_type)
    {
        case ShaderHolder::ShaderRenderType::textured:
            return "Textured";
        case ShaderHolder::ShaderRenderType::colored:
            return "Colored";
        case ShaderHolder::ShaderRenderType::count:
            return "Error";
        default:
            return "Unknown";
    }
}

static const char* get_component_type_str(Scene::ComponentType component)
{
    switch (component)
    {
        case Scene::ComponentType::transform:
            return "Transform";
        case Scene::ComponentType::static_mesh_render:
            return "Static Mesh Render";
        case Scene::ComponentType::camera:
            return "Camera";
        case Scene::ComponentType::count:
            return "Error";
        default:
            return "Unknown";
    }
}

static void show_input_box(const char* label, std::string& value)
{
    ImGui::Text("%s: ", label);
    char value_buffer[128]; // NOLINT(*-avoid-c-arrays)
    strncpy_s(value_buffer, value.c_str(), sizeof(value_buffer));
    value_buffer[sizeof(value_buffer) - 1] = '\0'; // Ensure null-termination
    if (ImGui::InputText(label, value_buffer, sizeof(value_buffer)))
    {
        value = value_buffer;
    }
}

static void show_readonly_input_box(const char* label, const std::string& value)
{
    ImGui::Text("%s: ", label);
    char value_buffer[128]; // NOLINT(*-avoid-c-arrays)
    strncpy_s(value_buffer, value.c_str(), sizeof(value_buffer));
    value_buffer[sizeof(value_buffer) - 1] = '\0'; // Ensure null-termination
    ImGui::InputText(label, value_buffer, sizeof(value_buffer), ImGuiInputTextFlags_ReadOnly);
}

bool ToolRenderer::create(ToolState& tool, SDL_Window* window, ID3D11Device2* device, ID3D11DeviceContext2* device_context)
{
    tool.pending_events.reserve(32);

    return tool.imgui.create(window, device, device_context);
}

void ToolRenderer::on_event(const SDL_Event* event)
{
    ImGui_ImplSDL3_ProcessEvent(event);
}

void ToolRenderer::update()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void ToolRenderer::render()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ToolRenderer::show_actor_properties(ToolState& tool, Scene::SceneState& scene)
{
    if (!tool.selected_actor_handle.is_valid())
    {
        return;
    }

    Scene::Actor* actor = Scene::get_actor(scene, tool.selected_actor_handle);

    if (actor == nullptr)
    {
        return;
    }

    ImGui::Begin("Actor Properties");

    // Name
    {
        show_input_box("Name", actor->name);
    }
    // ================

    // Transform
    {
        Vec3F pos = actor->transform.get_pos();
        ImGui::Text("Position:");
        ImGui::InputFloat("Pos X", &pos.x, 0.1f, 1.0f, "%.3f");
        ImGui::InputFloat("Pos Y", &pos.y, 0.1f, 1.0f, "%.3f");
        ImGui::InputFloat("Pos Z", &pos.z, 0.1f, 1.0f, "%.3f");
        actor->transform.set_pos(pos);

        Vec3F rot = actor->transform.get_rot();
        ImGui::Text("Rotation:");
        ImGui::InputFloat("Pitch", &rot.x, 0.1f, 1.0f, "%.3f");
        ImGui::InputFloat("Yaw", &rot.y, 0.1f, 1.0f, "%.3f");
        ImGui::InputFloat("Roll", &rot.z, 0.1f, 1.0f, "%.3f");
        actor->transform.set_rot(rot);

        Vec3F scale = actor->transform.get_scale();
        ImGui::Text("Scale:");
        ImGui::InputFloat("Scale X", &scale.x, 0.1f, 1.0f, "%.3f");
        ImGui::InputFloat("Scale Y", &scale.y, 0.1f, 1.0f, "%.3f");
        ImGui::InputFloat("Scale Z", &scale.z, 0.1f, 1.0f, "%.3f");
        actor->transform.set_scale(scale);
    }
    // ================

    // StaticMeshRender
    if (actor->mesh_render.has_value())
    {
        ImGui::Text("Mesh Render:");
        show_readonly_input_box("Mesh", uuid_to_string(actor->mesh_render->mesh_id));
        show_readonly_input_box("Texture", uuid_to_string(actor->mesh_render->texture_id));

        const char* current_shader_type = get_shader_type_str(actor->mesh_render->shader_type);
        if (ImGui::BeginCombo("Shader Type", current_shader_type))
        {
            for (int i = 0; i < (int)ShaderHolder::ShaderRenderType::count; ++i)
            {
                ShaderHolder::ShaderRenderType type = (ShaderHolder::ShaderRenderType)i;
                bool is_selected = (actor->mesh_render->shader_type == type);
                if (ImGui::Selectable(get_shader_type_str(type), is_selected))
                {
                    actor->mesh_render->shader_type = type;
                }
            }
            ImGui::EndCombo();
        }
    }
    // ================

    // Camera
    if (actor->camera.has_value())
    {
        ImGui::Text("Camera:");

        ImGui::InputFloat("VFov", &actor->camera->vfov, 1.0f, 10.0f, "%.3f");
        ImGui::InputFloat("Near", &actor->camera->near_plane, 0.01f, 0.1f, "%.3f");
        ImGui::InputFloat("Far", &actor->camera->far_plane, 0.1f, 10.0f, "%.3f");
    }
    // ================

    ImGui::NewLine();

    // Component Options
    {
        if (ImGui::BeginCombo("Add Component", ""))
        {
            for (Scene::ComponentType component : actor->missing_components())
            {
                if (ImGui::Selectable(get_component_type_str(component), false))
                {
                    tool.pending_events.emplace_back(ToolEvent::add_component(actor->handle, component));
                }
            }

            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo("Delete Component", ""))
        {
            for (Scene::ComponentType component : actor->added_components())
            {
                if (ImGui::Selectable(get_component_type_str(component), false))
                {
                    tool.pending_events.emplace_back(ToolEvent::delete_component(actor->handle, component));
                }
            }

            ImGui::EndCombo();
        }
    }
    // ================

    ImGui::End();
}

void ToolRenderer::show_scene_heirarchy(ToolState& tool, const Scene::SceneState& scene)
{
    ImGui::Begin("Scene Heirarchy");

    // Show scene tree
    {
        ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
        if (ImGui::TreeNodeEx("root", flag))
        {
            for (const Scene::Actor& actor : scene.actors)
            {
                if (!actor.is_alive)
                {
                    continue;
                }

                if (actor.parent_handle.is_valid())
                {
                    continue;
                }

                show_scene_entry(tool, scene, actor);
            }

            ImGui::TreePop();
        }
    }

    // Show scene options
    {
        ImGui::NewLine();

        if (ImGui::Button("Add Actor"))
        {
            tool.pending_events.emplace_back(ToolEvent::add_actor());
        }

        if (ImGui::Button("Delete Actor"))
        {
            if (tool.selected_actor_handle.is_valid())
            {
                tool.pending_events.emplace_back(ToolEvent::delete_actor(tool.selected_actor_handle));
            }
        }
    }

    ImGui::End();
}

void ToolRenderer::show_scene_entry(ToolState& tool, const Scene::SceneState& scene, const Scene::Actor& actor)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

    if (actor.children_handles.empty())
    {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (tool.selected_actor_handle.index == actor.handle.index)
    {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    if (ImGui::TreeNodeEx(actor.name.c_str(), flags))
    {
        if (ImGui::IsItemClicked())
        {
            tool.selected_actor_handle = actor.handle;
        }

        for (Scene::ActorHandle child_handle : actor.children_handles)
        {
            const Scene::Actor* child = Scene::get_actor(scene, child_handle);

            if (child == nullptr)
            {
                continue;
            }

            show_scene_entry(tool, scene, *child);
        }

        ImGui::TreePop();
    }
}
