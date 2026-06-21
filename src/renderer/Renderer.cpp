//
// Created by TheAwesomeGem on 11/29/2025.
//

#include "Renderer.h"

#include <ranges>

#include "RendererCommon.h"
#include "src/Config.h"
#include "src/Log.h"
#include "src/util/Mat4Ext.h"

using RendererCommon::LOG_CAT;

// TODO: Improve how we interact with constant buffer.

Renderer::Renderer()
    : m_debug{ },
      m_adapter{ },
      m_context{ },
      m_framebuffer{ },
      m_globalbuffer{ },
      m_shaderholder{ },
      m_textureholder{ },
      m_meshholder{ }
{
}

void Renderer::init(HWND window_handle)
{
    // Initialize Renderer Components
    {
        RenderDebug::create(m_debug);
        Adapter::create(m_adapter, m_debug);
        RenderContext::create(m_context, m_debug, m_adapter);
        Framebuffer::create(m_framebuffer, m_debug, m_adapter, m_context, window_handle);
        RenderGlobalBuffer::create(m_globalbuffer, m_debug, m_context);
        ShaderHolder::create(m_shaderholder, m_debug, m_context);
        TextureHolder::create(m_textureholder, m_debug, m_context);
        MeshHolder::create();
        Log::info(LOG_CAT, "Initialized renderer.");
    }
    // ===================

    // Setup initial pipeline
    {
        RenderContext::setup_initial_pipeline(m_context);
        Framebuffer::update_size(m_framebuffer, m_debug, m_context);
        RenderGlobalBuffer::setup_initial_pipeline(m_globalbuffer, m_context);
        TextureHolder::setup_initial_pipeline(m_textureholder, m_context);
        Log::info(LOG_CAT, "Setup initial pipeline.");
    }
    // ===================
}

void Renderer::begin_draw()
{
    Framebuffer::clear(m_framebuffer, m_context);
}

void Renderer::draw_static_mesh(const Mat4 view_proj_mat, const StaticMeshDrawData& mesh_data, const Mat4 world_transform)
{
    // TODO: Abstract it a bit more so that we don't have to use so much raw context here.

    // Constant Buffers
    {
        RenderGlobalBuffer::GlobalData drawable_data;
        drawable_data.pos_transform = (world_transform * view_proj_mat).Transpose();
        drawable_data.normal_transform = Mat4Ext::StripTranslation(world_transform).Invert();
        drawable_data.tint_color = mesh_data.tint_color;

        D3D11_MAPPED_SUBRESOURCE drawable_data_resource;
        RenderDebug::check(
            m_debug,
            m_context.context->Map(
                m_globalbuffer.constant_buffer.get(),
                0,
                D3D11_MAP_WRITE_DISCARD,
                0,
                &drawable_data_resource
            )
        );
        memcpy(drawable_data_resource.pData, &drawable_data, sizeof(drawable_data));
        m_context.context->Unmap(m_globalbuffer.constant_buffer.get(), 0);
    }
    // ===================

    // Shader
    {
        const ShaderHolder::ShaderState& shader = ShaderHolder::get_shader_state(m_shaderholder, (uint8_t)mesh_data.shader_type);
        m_context.context->IASetInputLayout(shader.input_layout.get());
        m_context.context->VSSetShader(shader.vertex.get(), nullptr, 0);
        m_context.context->PSSetShader(shader.pixel.get(), nullptr, 0);
    }
    // ===================

    // Texture
    {
        if (!mesh_data.texture_id.is_nil())
        {
            const TextureHolder::GpuTextureData& texture_state = m_textureholder.textures.at(mesh_data.texture_id);
            m_context.context->PSSetShaderResources(0, 1, texture_state.texture_view.addressof());
        }
        else
        {
            ID3D11ShaderResourceView* null_view = nullptr;
            m_context.context->PSSetShaderResources(0, 1, &null_view);
        }
    }
    // ===================

    // Mesh/Draw
    {
        const MeshHolder::GpuMeshData& mesh_state = m_meshholder.static_meshes.at(mesh_data.mesh_id); // TODO: Batch them
        m_context.context->IASetVertexBuffers(
            0,
            (uint32_t)mesh_state.vertex_buffers.size(),
            mesh_state.vertex_buffers.data()->addressof(),
            mesh_state.strides.data(),
            Common::NO_OFFSETS.data()
        );
        m_context.context->IASetIndexBuffer(mesh_state.index_buffer.get(), DXGI_FORMAT_R32_UINT, 0);
        m_context.context->DrawIndexed(mesh_state.index_count, 0, 0);
    }
    // ===================
}

void Renderer::end_draw()
{
    Framebuffer::present(m_framebuffer);
}

uuids::uuid Renderer::upload_texture(std::string_view texture_file_name)
{
    return TextureHolder::upload_texture(m_textureholder, m_debug, m_context, texture_file_name);
}

uuids::uuid Renderer::upload_mesh(const char* file_path)
{
    return MeshHolder::upload_mesh(m_meshholder, m_debug, m_context, file_path);
}

void Renderer::on_window_size_change()
{
    Framebuffer::update_size(m_framebuffer, m_debug, m_context);
}
