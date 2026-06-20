//
// Created by TheAwesomeGem on 11/29/2025.
//

#pragma once
#include "Adapter.h"
#include "Framebuffer.h"
#include "MeshHolder.h"
#include "RenderDebug.h"
#include "RenderGlobalBuffer.h"
#include "RendererCommon.h"
#include "ShaderHolder.h"
#include "TextureHolder.h"


struct ViewState
{
    Vec3F forward = Vec3F::UnitZ;
    Mat4 look_at_mat = Mat4::Identity;
};

struct StaticMeshDrawData
{
    uuids::uuid mesh_id = uuids::uuid{};
    uuids::uuid texture_id = uuids::uuid{};
    ShaderRenderType shader_type = ShaderRenderType::textured;
    Color tint_color = Color{ 1.0F, 1.0F, 1.0F, 1.0F };
};

// Used for rendering tools
struct D3DToolContext
{
    com_ptr<ID3D11Device2> m_device;
    com_ptr<ID3D11DeviceContext2> m_context;
};

class Renderer
{
public:
    NOT_COPYABLE_AND_MOVEABLE(Renderer);

    Renderer();
    void init(HWND window_handle);
    void begin_draw();
    void draw_static_mesh(Mat4 view_proj_mat, const StaticMeshDrawData& mesh_data, Mat4 world_transform);
    void end_draw();
    uuids::uuid upload_texture(std::string_view texture_file_name);
    uuids::uuid upload_mesh(const char* file_path);
    void on_window_size_change();

    [[nodiscard]] Vec2I get_viewport_size() const
    {
        return m_framebuffer.get_size();
    }

    [[nodiscard]] D3DToolContext get_tool_context() const
    {
        return D3DToolContext{ .m_device = m_context.m_device, .m_context = m_context.m_context };
    }

private:
    RenderDebug m_debug;
    Adapter m_adapter;
    RenderContext m_context;
    Framebuffer m_framebuffer;
    RenderGlobalBuffer m_globalbuffer;
    ShaderHolder m_shaderholder;
    TextureHolder m_textureholder;
    MeshHolder m_meshholder;
};
