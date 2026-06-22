//
// Created by TheAwesomeGem on 12/9/2025.
//

#include "Game.h"

#include <algorithm>

#include "src/Config.h"
#include "../renderer/MeshBuilder.h"
#include "src/Log.h"
#include "src/imgui/ToolRenderer.h"
#include "src/util/MathExt.h"
#include "src/util/RandomExt.h"
#include "src/util/Vec3Ext.h"

static constexpr const char* LOG_CAT = "Game";

Game::Game(Renderer::RendererState& renderer, ToolRenderer::ToolState& tool)
    : m_scene{},
      m_renderer{ &renderer },
      m_input{ nullptr },
      m_tool{ &tool },
      m_player_handle{ Scene::ActorHandle::invalid() },
      m_crate_handle{ Scene::ActorHandle::invalid() },
      m_is_tool_shown{ false }
{
}

void Game::create(Input::InputState& input)
{
    m_input = &input;
    Scene::load(m_scene);

    uuids::uuid cube_mesh_id = Renderer::upload_mesh(*m_renderer, "model/cube.obj");
    uuids::uuid f22_mesh_id = Renderer::upload_mesh(*m_renderer, "model/f22.obj");
    uuids::uuid suzanne_mesh_id = Renderer::upload_mesh(*m_renderer, "model/suzanne.obj");

    uuids::uuid crate_texture_id = Renderer::upload_texture(*m_renderer, "texture/crate.png");
    uuids::uuid uv_test_texture_id = Renderer::upload_texture(*m_renderer, "texture/uv_test.png");

    // Hook up tools
    // if (m_tool){
    //     m_tool->m_add_actor_callback = [this]()
    //     {
    //         m_scene.spawn_actor(std::format("Actor {}", RandomExt::range(0, 99999)));
    //     };
    //
    //     m_tool->m_delete_actor_callback = [this](ActorHandle handle)
    //     {
    //         m_scene.destroy_actor(handle, true);
    //     };
    //
    //     m_tool->m_add_component_callback = [this, cube_mesh_id, crate_texture_id](ActorHandle handle, ComponentType component)
    //     {
    //         Actor* actor = m_scene.get_actor(handle);
    //
    //         if (actor == nullptr)
    //         {
    //             return;
    //         }
    //
    //         switch (component)
    //         {
    //             case ComponentType::static_mesh_render:
    //             {
    //                 m_scene.attach_mesh_render(
    //                     actor->handle, m_scene.create_mesh_render(
    //                         cube_mesh_id, crate_texture_id, ShaderRenderType::textured, Color{ 1.0F, 1.0F, 1.0F, 1.0F }
    //                     )
    //                 );
    //                 break;
    //             }
    //             case ComponentType::camera:
    //             {
    //                 m_scene.attach_camera(actor->handle, m_scene.create_projected_camera(Config::VFOV, 0.1F, 100.0F));
    //                 break;
    //             }
    //             case ComponentType::transform:
    //             case ComponentType::count:
    //             default:
    //             {
    //                 break;
    //             }
    //         }
    //     };
    //
    //     m_tool->m_delete_component_callback = [this](ActorHandle handle, ComponentType component)
    //     {
    //         Actor* actor = m_scene.get_actor(handle);
    //
    //         if (actor == nullptr)
    //         {
    //             return;
    //         }
    //
    //         switch (component)
    //         {
    //             case ComponentType::static_mesh_render:
    //             {
    //                 actor->mesh_render = std::nullopt;
    //                 break;
    //             }
    //             case ComponentType::camera:
    //             {
    //                 actor->camera = std::nullopt;
    //                 break;
    //             }
    //             case ComponentType::transform:
    //             case ComponentType::count:
    //             default:
    //             {
    //                 break;
    //             }
    //         }
    //     };
    // }

    // Spawn actors
    {
        m_player_handle = Scene::spawn_actor(m_scene, "Player", Vec3F{ 0.0F, 0.0F, -5.0F });
        Scene::ActorHandle cam_actor_handle = Scene::spawn_actor(
            m_scene, "Camera", Vec3F{ 0.0F, 3.0F, 0.0F }, Vec3F::Zero, Vec3F::One, m_player_handle
        );
        Scene::attach_camera(
            m_scene, cam_actor_handle, Scene::create_projected_camera(Config::VFOV, 0.1F, 100.0F)
        );
        m_scene.camera_handle = cam_actor_handle;

        m_crate_handle = Scene::spawn_actor(m_scene, "Crate", Vec3F{ 0.0F, 0.5F, 2.0F }, Vec3F::Zero, Vec3F{ 0.5F, 0.5F, 0.5F });
        Scene::attach_mesh_render(
            m_scene,
            m_crate_handle, Scene::create_mesh_render(
                suzanne_mesh_id, uv_test_texture_id, ShaderHolder::ShaderRenderType::textured, Color{ 1.0F, 1.0F, 1.0F, 1.0F }
            )
        );

        Scene::ActorHandle floor_handle = Scene::spawn_actor(
            m_scene, "Floor", Vec3F{ 0.0F, -1.0F, 0.0F }, Vec3F::Zero, Vec3F{ 50.0F, 1.0F, 50.0F }
        );
        Scene::attach_mesh_render(
            m_scene,
            floor_handle, Scene::create_mesh_render(
                cube_mesh_id, uuids::uuid{}, ShaderHolder::ShaderRenderType::colored, Color{ 1.0F, 1.0F, 1.0F, 1.0F }
            )
        );
    }
    // ================
}

void Game::update(const GameWindow::WindowState& window, double delta_time)
{
    Scene::update(m_scene, delta_time);

    if (m_input == nullptr)
    {
        Log::warn(LOG_CAT, "Input subsystem is not ready yet");

        return;
    }

    // Tooling Controls
    {
        if (Input::is_key_just_pressed(*m_input, SDL_SCANCODE_ESCAPE))
        {
            m_is_tool_shown = !m_is_tool_shown;
            Input::enable_mouse_cursor(window, m_is_tool_shown);
        }
    }
    // ===============

    // Crate rotation
    {
        Scene::Actor* crate = Scene::get_actor(m_scene, m_crate_handle);
        if (crate != nullptr)
        {
            crate->transform.rot_pitch(45.0F * (float)delta_time);
            crate->transform.rot_yaw(60.0F * (float)delta_time);
        }
    }
    // ===============

    Scene::Actor* player = Scene::get_actor(m_scene, m_player_handle);

    if (player != nullptr && !m_is_tool_shown)
    {
        // Player/Camera Rotation
        {
            constexpr float vfov = DirectX::XMConvertToRadians(Config::VFOV);
            constexpr float sensitivity = Config::LOOK_SENSITIVITY;
            const auto& [viewport_width, viewport_height] = m_renderer->framebuffer.size;
            const float aspect = (float)(viewport_width) / (float)(viewport_height);
            const float hfov = 2.0F * atanf(tanf(vfov * 0.5F) * aspect);
            const float yaw_per_px = hfov / (float)(viewport_width);
            const float pitch_per_px = vfov / (float)(viewport_height);
            const Vec2F mouse_delta = m_input->mouse.delta;

            Scene::Actor* camera = Scene::get_actor(m_scene, player->children_handles[0]);

            if (camera != nullptr)
            {
                Vec3F player_rot = player->transform.get_rot();
                const float yaw_deg = player_rot.y + (mouse_delta.x * yaw_per_px * sensitivity);
                player_rot.y = yaw_deg;
                player->transform.set_rot(player_rot);

                Vec3F camera_rot = camera->transform.get_rot();
                float pitch_deg = camera_rot.x + (mouse_delta.y * pitch_per_px * sensitivity);
                pitch_deg = std::clamp(pitch_deg, -89.0F, 89.0F);
                camera_rot.x = pitch_deg;
                camera->transform.set_rot(camera_rot);
            }
        }
        // ===============

        // Player Movement
        {
            float fwd = 0.0F;
            float rgt = 0.0F;

            if (Input::is_key_down(*m_input, SDL_SCANCODE_W))
            {
                fwd += 1.0F;
            }
            if (Input::is_key_down(*m_input, SDL_SCANCODE_S))
            {
                fwd -= 1.0F;
            }
            if (Input::is_key_down(*m_input, SDL_SCANCODE_D))
            {
                rgt += 1.0F;
            }
            if (Input::is_key_down(*m_input, SDL_SCANCODE_A))
            {
                rgt -= 1.0F;
            }

            Vec2F move_dir = Vec2F{ rgt, fwd };
            if (move_dir.LengthSquared() > 1.0F)
            {
                move_dir.Normalize();
            }

            Vec3F fwd_dir = Vec3Ext::xz_plane(player->transform.get_mat().Backward());
            fwd_dir.Normalize();
            Vec3F right_dir = Vec3Ext::xz_plane(Vec3F::UnitY.Cross(fwd_dir));
            right_dir.Normalize();
            const Vec3F displacement = (right_dir * move_dir.x + fwd_dir * move_dir.y) * Config::MOVEMENT_SPEED * (float)delta_time;

            player->transform.translate(displacement);
        }
        // =================

        // TEST REMOVE
        {
            if (Input::is_key_down(*m_input, SDL_SCANCODE_F))
            {
                Scene::destroy_actor(m_scene, m_crate_handle, true);
            }
        }
        // =================
    }

    ToolRenderer::update();
}

void Game::render()
{
    Renderer::begin_draw(*m_renderer);
    Scene::render(m_scene, *m_renderer);
    if (m_tool && m_is_tool_shown)
    {
        ToolRenderer::show_scene_heirarchy(*m_tool, m_scene);
        ToolRenderer::show_actor_properties(*m_tool, m_scene);
    }
    ToolRenderer::render();
    Renderer::end_draw(*m_renderer);
}
