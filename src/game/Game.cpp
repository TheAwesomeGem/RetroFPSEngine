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

void Game::create(GameState& game, Renderer::RendererState& renderer, Input::InputState& input, ToolRenderer::ToolState& tool)
{
    game.renderer = &renderer;
    game.tool = &tool;
    game.input = &input;
    Scene::load(game.scene);

    game.cube_mesh_id = Renderer::upload_mesh(*game.renderer, "model/cube.obj");
    game.f22_mesh_id = Renderer::upload_mesh(*game.renderer, "model/f22.obj");
    game.suzanne_mesh_id = Renderer::upload_mesh(*game.renderer, "model/suzanne.obj");

    game.crate_texture_id = Renderer::upload_texture(*game.renderer, "texture/crate.png");
    game.uv_test_texture_id = Renderer::upload_texture(*game.renderer, "texture/uv_test.png");

    // Spawn actors
    {
        game.player_handle = Scene::spawn_actor(game.scene, "Player", Vec3F{ 0.0F, 0.0F, -5.0F });
        Scene::ActorHandle cam_actor_handle = Scene::spawn_actor(
            game.scene, "Camera", Vec3F{ 0.0F, 3.0F, 0.0F }, Vec3F::Zero, Vec3F::One, game.player_handle
        );
        Scene::attach_camera(
            game.scene, cam_actor_handle, Scene::create_projected_camera(Config::VFOV, 0.1F, 100.0F)
        );
        game.scene.camera_handle = cam_actor_handle;

        game.crate_handle = Scene::spawn_actor(game.scene, "Crate", Vec3F{ 0.0F, 0.5F, 2.0F }, Vec3F::Zero, Vec3F{ 0.5F, 0.5F, 0.5F });
        Scene::attach_mesh_render(
            game.scene,
            game.crate_handle, Scene::create_mesh_render(
                game.suzanne_mesh_id, game.uv_test_texture_id, ShaderHolder::ShaderRenderType::textured, Color{ 1.0F, 1.0F, 1.0F, 1.0F }
            )
        );

        Scene::ActorHandle floor_handle = Scene::spawn_actor(
            game.scene, "Floor", Vec3F{ 0.0F, -1.0F, 0.0F }, Vec3F::Zero, Vec3F{ 50.0F, 1.0F, 50.0F }
        );
        Scene::attach_mesh_render(
            game.scene,
            floor_handle, Scene::create_mesh_render(
                game.cube_mesh_id, uuids::uuid{}, ShaderHolder::ShaderRenderType::colored, Color{ 1.0F, 1.0F, 1.0F, 1.0F }
            )
        );
    }
    // ================
}

void Game::update(GameState& game, const GameWindow::WindowState& window, double delta_time)
{
    Scene::update(game.scene, delta_time);

    if (game.input == nullptr)
    {
        Log::warn(LOG_CAT, "Input subsystem is not ready yet");

        return;
    }

    // Tooling Controls
    {
        if (Input::is_key_just_pressed(*game.input, SDL_SCANCODE_ESCAPE))
        {
            game.is_tool_shown = !game.is_tool_shown;
            Input::enable_mouse_cursor(window, game.is_tool_shown);
        }
    }
    // ===============

    // Crate rotation
    {
        Scene::Actor* crate = Scene::get_actor(game.scene, game.crate_handle);
        if (crate != nullptr)
        {
            crate->transform.rot_pitch(45.0F * (float)delta_time);
            crate->transform.rot_yaw(60.0F * (float)delta_time);
        }
    }
    // ===============

    Scene::Actor* player = Scene::get_actor(game.scene, game.player_handle);

    if (player != nullptr && !game.is_tool_shown)
    {
        // Player/Camera Rotation
        {
            constexpr float vfov = DirectX::XMConvertToRadians(Config::VFOV);
            constexpr float sensitivity = Config::LOOK_SENSITIVITY;
            const auto& [viewport_width, viewport_height] = game.renderer->framebuffer.size;
            const float aspect = (float)(viewport_width) / (float)(viewport_height);
            const float hfov = 2.0F * atanf(tanf(vfov * 0.5F) * aspect);
            const float yaw_per_px = hfov / (float)(viewport_width);
            const float pitch_per_px = vfov / (float)(viewport_height);
            const Vec2F mouse_delta = game.input->mouse.delta;

            Scene::Actor* camera = Scene::get_actor(game.scene, player->children_handles[0]);

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

            if (Input::is_key_down(*game.input, SDL_SCANCODE_W))
            {
                fwd += 1.0F;
            }
            if (Input::is_key_down(*game.input, SDL_SCANCODE_S))
            {
                fwd -= 1.0F;
            }
            if (Input::is_key_down(*game.input, SDL_SCANCODE_D))
            {
                rgt += 1.0F;
            }
            if (Input::is_key_down(*game.input, SDL_SCANCODE_A))
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
            if (Input::is_key_down(*game.input, SDL_SCANCODE_F))
            {
                Scene::destroy_actor(game.scene, game.crate_handle, true);
            }
        }
        // =================
    }

    ToolRenderer::update();

    // Handle tool events
    if (game.tool)
    {
        for (const ToolRenderer::ToolEvent& event : game.tool->pending_events)
        {
            switch (event.type)
            {
                case ToolRenderer::ToolEvent::Type::AddActor:
                {
                    Scene::spawn_actor(game.scene, std::format("Actor {}", RandomExt::range(0, 99999)));
                    break;
                }
                case ToolRenderer::ToolEvent::Type::DeleteActor:
                {
                    Scene::destroy_actor(game.scene, event.actor_data.handle, true);
                    break;
                }
                case ToolRenderer::ToolEvent::Type::AddComponent:
                {
                    Scene::Actor* actor = Scene::get_actor(game.scene, event.actor_component_data.handle);

                    if (actor == nullptr)
                    {
                        return;
                    }

                    switch (event.actor_component_data.component)
                    {
                        case Scene::ComponentType::static_mesh_render:
                        {
                            Scene::attach_mesh_render(
                                game.scene, actor->handle, Scene::create_mesh_render(
                                    game.cube_mesh_id, game.crate_texture_id, ShaderHolder::ShaderRenderType::textured, Color{ 1.0F, 1.0F, 1.0F, 1.0F }
                                )
                            );
                            break;
                        }
                        case Scene::ComponentType::camera:
                        {
                            Scene::attach_camera(game.scene, actor->handle, Scene::create_projected_camera(Config::VFOV, 0.1F, 100.0F));
                            break;
                        }
                        case Scene::ComponentType::transform:
                        case Scene::ComponentType::count:
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case ToolRenderer::ToolEvent::Type::DeleteComponent:
                {
                    Scene::Actor* actor = Scene::get_actor(game.scene, event.actor_component_data.handle);

                    if (actor == nullptr)
                    {
                        return;
                    }

                    switch (event.actor_component_data.component)
                    {
                        case Scene::ComponentType::static_mesh_render:
                        {
                            actor->mesh_render = std::nullopt;
                            break;
                        }
                        case Scene::ComponentType::camera:
                        {
                            actor->camera = std::nullopt;
                            break;
                        }
                        case Scene::ComponentType::transform:
                        case Scene::ComponentType::count:
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case ToolRenderer::ToolEvent::Type::Unknown:
                {
                    // nothing to do

                    break;
                }
            }
        }

        game.tool->pending_events.clear();
    }
}

void Game::render(GameState& game)
{
    Renderer::begin_draw(*game.renderer);
    Scene::render(game.scene, *game.renderer);
    if (game.tool && game.is_tool_shown)
    {
        ToolRenderer::show_scene_heirarchy(*game.tool, game.scene);
        ToolRenderer::show_actor_properties(*game.tool, game.scene);
    }
    ToolRenderer::render();
    Renderer::end_draw(*game.renderer);
}
