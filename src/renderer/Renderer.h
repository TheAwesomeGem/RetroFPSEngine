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


namespace Renderer
{
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

    struct RendererState
    {
        NOT_COPYABLE_AND_MOVEABLE(RendererState);
        RendererState() = default;

        RenderDebug::DebugState debug = RenderDebug::DebugState{};
        Adapter::AdapterState adapter = Adapter::AdapterState{};
        RenderContext::ContextState context = RenderContext::ContextState{};
        Framebuffer::BufferState framebuffer = Framebuffer::BufferState{};
        RenderGlobalBuffer::BufferState globalbuffer = RenderGlobalBuffer::BufferState{};
        ShaderHolder::HolderState shaderholder = ShaderHolder::HolderState{};
        TextureHolder::HolderState textureholder = TextureHolder::HolderState{};
        MeshHolder::HolderState meshholder = MeshHolder::HolderState{};
    };

    void create(RendererState& renderer, HWND window_handle);
    void begin_draw(const RendererState& renderer);
    void draw_static_mesh(const RendererState& renderer, Mat4 view_proj_mat, const StaticMeshDrawData& mesh_data, Mat4 world_transform);
    void end_draw(const RendererState& renderer);
    uuids::uuid upload_texture(RendererState& renderer, std::string_view texture_file_name);
    uuids::uuid upload_mesh(RendererState& renderer, const char* file_path);
    void on_window_size_change(RendererState& renderer);
    [[nodiscard]] inline D3DToolContext get_tool_context(RendererState& renderer)
    {
        return D3DToolContext{ .m_device = renderer.context.device, .m_context = renderer.context.context };
    }
}
