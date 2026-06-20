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
    : m_debug{},
      m_adapter{ &m_debug },
      m_context{ &m_debug, &m_adapter },
      m_framebuffer{ &m_debug, &m_adapter, &m_context },
      m_globalbuffer{ &m_debug, &m_context },
      m_shaderholder{ &m_debug, &m_context },
      m_textureholder{ &m_debug, &m_context },
      m_meshholder{ &m_debug, &m_context }
{
}

void Renderer::init(HWND window_handle)
{
    // Initialize Renderer Components
    {
        m_debug.init();
        m_adapter.init();
        m_context.init();
        m_framebuffer.init(window_handle);
        m_globalbuffer.init();
        m_shaderholder.init();
        m_textureholder.init();
        m_meshholder.init();
        Log::info(LOG_CAT, "Initialized renderer.");
    }
    // ===================

    // Setup initial pipeline
    {
        m_context.setup_initial_pipeline();
        m_framebuffer.update_size();
        m_globalbuffer.setup_initial_pipeline();
        m_textureholder.setup_initial_pipeline();
        Log::info(LOG_CAT, "Setup initial pipeline.");
    }
    // ===================
}

void Renderer::begin_draw()
{
    m_framebuffer.clear();
}

void Renderer::draw_static_mesh(const Mat4 view_proj_mat, const StaticMeshDrawData& mesh_data, const Mat4 world_transform)
{
    // TODO: Abstract it a bit more so that we don't have to use so much raw context here.

    // Constant Buffers
    {
        GlobalData drawable_data;
        drawable_data.pos_transform = (world_transform * view_proj_mat).Transpose();
        drawable_data.normal_transform = Mat4Ext::StripTranslation(world_transform).Invert();
        drawable_data.tint_color = mesh_data.tint_color;

        D3D11_MAPPED_SUBRESOURCE drawable_data_resource;
        m_debug.check(
            m_context.m_context->Map(
                m_globalbuffer.m_constant_buffer.get(),
                0,
                D3D11_MAP_WRITE_DISCARD,
                0,
                &drawable_data_resource
            )
        );
        memcpy(drawable_data_resource.pData, &drawable_data, sizeof(drawable_data));
        m_context.m_context->Unmap(m_globalbuffer.m_constant_buffer.get(), 0);
    }
    // ===================

    // Shader
    {
        const ShaderState& shader = m_shaderholder.get_shader_state((uint8_t)mesh_data.shader_type);
        m_context.m_context->IASetInputLayout(shader.input_layout.get());
        m_context.m_context->VSSetShader(shader.vertex.get(), nullptr, 0);
        m_context.m_context->PSSetShader(shader.pixel.get(), nullptr, 0);
    }
    // ===================

    // Texture
    {
        if (!mesh_data.texture_id.is_nil())
        {
            const GpuTextureData& texture_state = m_textureholder.get_texture_data(mesh_data.texture_id);
            m_context.m_context->PSSetShaderResources(0, 1, texture_state.texture_view.addressof());
        }
        else
        {
            ID3D11ShaderResourceView* null_view = nullptr;
            m_context.m_context->PSSetShaderResources(0, 1, &null_view);
        }
    }
    // ===================

    // Mesh/Draw
    {
        const GpuMeshData& mesh_state = m_meshholder.get_static_mesh(mesh_data.mesh_id); // TODO: Batch them
        m_context.m_context->IASetVertexBuffers(
            0,
            (uint32_t)mesh_state.vertex_buffers.size(),
            mesh_state.vertex_buffers.data()->addressof(),
            mesh_state.strides.data(),
            Common::NO_OFFSETS.data()
        );
        m_context.m_context->IASetIndexBuffer(mesh_state.index_buffer.get(), DXGI_FORMAT_R32_UINT, 0);
        m_context.m_context->DrawIndexed(mesh_state.index_count, 0, 0);
    }
    // ===================
}

void Renderer::end_draw()
{
    m_framebuffer.present();
}

uuids::uuid Renderer::upload_texture(std::string_view texture_file_name)
{
    return m_textureholder.upload_texture(texture_file_name);
}

uuids::uuid Renderer::upload_mesh(const char* file_path)
{
    return m_meshholder.upload_mesh(file_path);
}

void Renderer::on_window_size_change()
{
    m_framebuffer.update_size();
}
