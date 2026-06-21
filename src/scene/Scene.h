//
// Created by TheAwesomeGem on 12/2/2025.
//

#pragma once
#include <optional>
#include <print>
#include <unordered_set>

#include "src/Config.h"
#include "src/renderer/Renderer.h"
#include "src/util/MathExt.h"


enum class ComponentType: uint8_t
{
    transform,
    static_mesh_render,
    camera,
    count
};

struct Transform
{
    [[nodiscard]] Mat4 get_mat() const
    {
        return m_matrix_cache;
    }

    void recalculate_mat()
    {
        m_matrix_cache = Mat4::Identity *
        Mat4::CreateScale(m_scale) *
        Mat4::CreateRotationZ(m_rot.z) * Mat4::CreateRotationY(m_rot.y) * Mat4::CreateRotationX(m_rot.x) *
        Mat4::CreateTranslation(m_pos);
    }

    void set_rot(const Vec3F new_rot)
    {
        if (new_rot == get_rot())
        {
            return;
        }

        this->m_rot = new_rot;
        recalculate_mat();
    }

    void set_scale(const Vec3F new_scale)
    {
        if (new_scale == get_scale())
        {
            return;
        }

        this->m_scale = new_scale;
        recalculate_mat();
    }

    void set_pos(const Vec3F new_pos)
    {
        if (new_pos == get_pos())
        {
            return;
        }

        this->m_pos = new_pos;
        recalculate_mat();
    }

    [[nodiscard]] Vec3F get_rot() const
    {
        return this->m_rot;
    }

    [[nodiscard]] Vec3F get_scale() const
    {
        return this->m_scale;
    }

    [[nodiscard]] Vec3F get_pos() const
    {
        return this->m_pos;
    }

    void rot_pitch(const float amount)
    {
        Vec3F new_rot = get_rot();
        new_rot.x = MathExt::rot_mod(new_rot.x, amount);

        set_rot(new_rot);
    }

    void rot_yaw(const float amount)
    {
        Vec3F new_rot = get_rot();
        new_rot.y = MathExt::rot_mod(new_rot.y, amount);

        set_rot(new_rot);
    }

    void rot_roll(const float amount)
    {
        Vec3F new_rot = get_rot();
        new_rot.z = MathExt::rot_mod(new_rot.z, amount);

        set_rot(new_rot);
    }

    void uniform_scale(const float scalar)
    {
        set_scale(Vec3F{ scalar, scalar, scalar });
    }

    void translate(const Vec3F displacement)
    {
        set_pos(get_pos() + displacement);
    }

private:
    Vec3F m_pos = Vec3F::Zero;
    Vec3F m_rot = Vec3F::Zero;
    Vec3F m_scale = Vec3F::One;
    Mat4 m_matrix_cache = Mat4::Identity;
};

struct StaticMeshRender
{
    uuids::uuid mesh_id = uuids::uuid{};
    uuids::uuid texture_id = uuids::uuid{};
    ShaderHolder::ShaderRenderType shader_type = ShaderHolder::ShaderRenderType::textured;
    Color tint_color = Color{ 1.0F, 1.0F, 1.0F, 1.0F };
};

struct Camera
{
    float vfov = Config::VFOV;
    float near_plane = 0.1F;
    float far_plane = 100.0F;
};

struct ActorHandle
{
    size_t index = Common::INVALID_ACTOR_IDX;
    uint64_t generation = 0;

    static constexpr ActorHandle invalid()
    {
        return ActorHandle{ .index = Common::INVALID_ACTOR_IDX, .generation = 0 };
    }

    [[nodiscard]] bool is_valid() const
    {
        return index != Common::INVALID_ACTOR_IDX;
    }
};

struct Actor
{
    ActorHandle handle = ActorHandle::invalid();
    ActorHandle parent_handle = ActorHandle::invalid();
    std::string name = "";
    bool is_alive = false;
    std::vector<ActorHandle> children_handles = {};
    // Components
    Transform transform = {};
    std::optional<StaticMeshRender> mesh_render = std::nullopt;
    std::optional<Camera> camera = std::nullopt;
    // ==========

    std::vector<ComponentType> added_components()
    {
        std::vector<ComponentType> components = {};

        if (mesh_render.has_value())
        {
            components.emplace_back(ComponentType::static_mesh_render);
        }

        if (camera.has_value())
        {
            components.emplace_back(ComponentType::camera);
        }

        return components;
    }

    std::vector<ComponentType> missing_components()
    {
        std::vector<ComponentType> components = {};

        if (!mesh_render.has_value())
        {
            components.emplace_back(ComponentType::static_mesh_render);
        }

        if (!camera.has_value())
        {
            components.emplace_back(ComponentType::camera);
        }

        return components;
    }
};

class Scene
{
public:
    explicit Scene();

    void load();
    void update(double delta_time);
    void render(Renderer* renderer);
    StaticMeshRender create_mesh_render(uuids::uuid mesh_id, uuids::uuid texture_id, ShaderHolder::ShaderRenderType shader_type, Color tint_color);
    Camera create_projected_camera(float v_fov, float near_plane, float far_plane);
    ActorHandle spawn_actor(
        std::string name,
        Vec3F pos = Vec3F::Zero,
        Vec3F rot = Vec3F::Zero,
        Vec3F scale = Vec3F::One,
        ActorHandle parent_handle = ActorHandle::invalid()
    );
    void destroy_actor(ActorHandle handle, bool should_destroy_childrens);
    [[nodiscard]] Mat4 get_local_to_world_mat(const Actor* actor) const;

    void attach_mesh_render(ActorHandle handle, const StaticMeshRender& mesh_render)
    {
        Actor* actor = get_actor(handle);
        assert(actor != nullptr);

        actor->mesh_render = mesh_render;
    }

    void attach_camera(ActorHandle handle, const Camera& camera)
    {
        Actor* actor = get_actor(handle);
        assert(actor != nullptr);

        actor->camera = camera;
    }

    void set_camera_actor(ActorHandle handle)
    {
        m_camera_handle = handle;
    }

    Actor* get_actor(ActorHandle handle)
    {
        if (!is_actor_exists(handle))
        {
            return nullptr;
        }

        return &m_actors[handle.index];
    }

    [[nodiscard]] const Actor* get_actor(ActorHandle handle) const
    {
        if (!is_actor_exists(handle))
        {
            return nullptr;
        }

        return &m_actors[handle.index];
    }

    [[nodiscard]] bool is_actor_exists(ActorHandle handle) const
    {
        if (!handle.is_valid())
        {
            return false;
        }

        if (handle.index >= std::size(m_actors))
        {
            return false;
        }

        const Actor& actor = m_actors[handle.index];

        if (!actor.is_alive)
        {
            return false;
        }

        return actor.handle.generation == handle.generation;
    }

    [[nodiscard]] const std::vector<Actor>& get_actors() const
    {
        return m_actors;
    }

    [[nodiscard]] std::vector<Actor>& get_actors()
    {
        return m_actors;
    }

private:
    std::vector<Actor> m_actors; // TODO: Use a tree or something. This will be slow and heavy.
    std::vector<size_t> m_actor_free_indices;
    ActorHandle m_camera_handle;
};
