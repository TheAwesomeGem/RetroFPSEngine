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
    ShaderHolder::ShaderRenderType shader_type = ShaderHolder::ShaderRenderType::textured;
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
        return m_framebuffer.size;
    }

    [[nodiscard]] D3DToolContext get_tool_context() const
    {
        return D3DToolContext{ .m_device = m_context.device, .m_context = m_context.context };
    }

private:
    RenderDebug::DebugState m_debug = RenderDebug::DebugState{};
    Adapter::AdapterState m_adapter = Adapter::AdapterState{};
    RenderContext::ContextState m_context = RenderContext::ContextState{};
    Framebuffer::BufferState m_framebuffer = Framebuffer::BufferState{};
    RenderGlobalBuffer::BufferState m_globalbuffer = RenderGlobalBuffer::BufferState{};
    ShaderHolder::HolderState m_shaderholder = ShaderHolder::HolderState{};
    TextureHolder::HolderState m_textureholder = TextureHolder::HolderState{};
    MeshHolder::HolderState m_meshholder = MeshHolder::HolderState{};
};
