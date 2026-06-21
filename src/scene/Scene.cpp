//
// Created by TheAwesomeGem on 12/2/2025.
//

#include "Scene.h"

#include <algorithm>
#include <ranges>

#include "src/Config.h"
#include "src/Log.h"
#include "src/util/Mat4Ext.h"
#include "src/util/MathExt.h"

Scene::Scene()
    : m_actors{}, m_actor_free_indices{}, m_camera_handle{ ActorHandle::invalid() }
{
}

void Scene::load()
{
}

void Scene::update(double /*delta_time*/)
{
}

void Scene::render(Renderer* renderer)
{
    Mat4 view_proj_mat = Mat4::Identity;

    Actor* camera_actor = get_actor(m_camera_handle);
    if (camera_actor != nullptr)
    {
        auto [render_width, render_height] = renderer->get_viewport_size();
        Mat4 proj = Mat4Ext::CreatePerspectiveFieldOfViewLH(
            DirectX::XMConvertToRadians(camera_actor->camera->vfov),
            (float)render_width / (float)render_height,
            camera_actor->camera->near_plane,
            camera_actor->camera->far_plane
        );

        view_proj_mat = get_local_to_world_mat(camera_actor).Invert() * proj;
    }

    for (const Actor& actor : m_actors)
    {
        if (!is_actor_exists(actor.handle))
        {
            continue;
        }

        if (!actor.mesh_render.has_value())
        {
            continue;
        }

        renderer->draw_static_mesh(
            view_proj_mat,
            StaticMeshDrawData{
                .mesh_id = actor.mesh_render->mesh_id,
                .texture_id = actor.mesh_render->texture_id,
                .shader_type = actor.mesh_render->shader_type,
                .tint_color = actor.mesh_render->tint_color
            },
            get_local_to_world_mat(&actor)
        );
    }
}

StaticMeshRender Scene::create_mesh_render(uuids::uuid mesh_id, uuids::uuid texture_id, ShaderHolder::ShaderRenderType shader_type, Color tint_color)
{
    return StaticMeshRender{
        .mesh_id = mesh_id,
        .texture_id = texture_id,
        .shader_type = shader_type,
        .tint_color = tint_color
    };
}

Camera Scene::create_projected_camera(float v_fov, float near_plane, float far_plane)
{
    return Camera{
        .vfov = v_fov,
        .near_plane = near_plane,
        .far_plane = far_plane
    };
}

ActorHandle Scene::spawn_actor(std::string name, Vec3F pos, Vec3F rot, Vec3F scale, ActorHandle parent_handle)
{
    size_t index = 0;
    size_t generation = 0;
    bool should_emplace = false;
    if (!m_actor_free_indices.empty())
    {
        index = m_actor_free_indices.back();
        m_actor_free_indices.pop_back();
        generation = m_actors[index].handle.generation;
    }
    else
    {
        index = std::size(m_actors);
        should_emplace = true;
    }

    ActorHandle handle = ActorHandle{ .index = index, .generation = generation };
    Actor& actor = should_emplace ? m_actors.emplace_back() : m_actors[index];
    actor = Actor{ .handle = handle, .parent_handle = parent_handle, .name = std::move(name), .is_alive = true };
    actor.transform.set_pos(pos);
    actor.transform.set_rot(rot);
    actor.transform.set_scale(scale);
    actor.parent_handle = parent_handle;

    Actor* parent = get_actor(parent_handle);
    if (parent != nullptr)
    {
        parent->children_handles.emplace_back(handle);
    }

    return handle;
}

void Scene::destroy_actor(ActorHandle handle, bool should_destroy_childrens)
{
    Actor* actor = get_actor(handle);

    if (actor == nullptr)
    {
        return;
    }

    actor->is_alive = false;
    ++actor->handle.generation;

    // Clean up childrens (destroy or deattach)
    {
        for (ActorHandle child_handle : actor->children_handles)
        {
            Actor* child = get_actor(child_handle);

            if (child == nullptr)
            {
                continue;
            }

            child->parent_handle = ActorHandle::invalid();

            if (should_destroy_childrens)
            {
                destroy_actor(child_handle, true);
            }
        }

        actor->children_handles.clear();
    }
    // ===================

    // Deattch from parent
    {
        Actor* parent = get_actor(actor->parent_handle);

        if (parent != nullptr)
        {
            int removal_idx = -1;
            for (int i = 0; i < std::size(parent->children_handles); ++i)
            {
                ActorHandle children_handle = parent->children_handles[i];

                if (children_handle.index == actor->handle.index)
                {
                    removal_idx = i;

                    break;
                }
            }

            if (removal_idx != -1)
            {
                parent->children_handles.erase(parent->children_handles.begin() + removal_idx);
            }
        }
    }
    // ===================

    m_actor_free_indices.emplace_back(actor->handle.index);
}

Mat4 Scene::get_local_to_world_mat(const Actor* actor) const
{
    assert(actor != nullptr);

    Mat4 final_mat = actor->transform.get_mat();
    ActorHandle next_parent_handle = actor->parent_handle;

    const Actor* next_parent = nullptr;
    while ((next_parent = get_actor(next_parent_handle)) != nullptr)
    {
        final_mat = final_mat * next_parent->transform.get_mat();
        next_parent_handle = next_parent->parent_handle;
    }

    return final_mat;
}
