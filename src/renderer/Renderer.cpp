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

void Renderer::create(RendererState& renderer, HWND window_handle)
{
    // Initialize Renderer Components
    {
        RenderDebug::create(renderer.debug);
        Adapter::create(renderer.adapter, renderer.debug);
        RenderContext::create(renderer.context, renderer.debug, renderer.adapter);
        Framebuffer::create(renderer.framebuffer, renderer.debug, renderer.adapter, renderer.context, window_handle);
        RenderGlobalBuffer::create(renderer.globalbuffer, renderer.debug, renderer.context);
        ShaderHolder::create(renderer.shaderholder, renderer.debug, renderer.context);
        TextureHolder::create(renderer.textureholder, renderer.debug, renderer.context);
        MeshHolder::create();
        Log::info(LOG_CAT, "Initialized renderer.");
    }
    // ===================

    // Setup initial pipeline
    {
        RenderContext::setup_initial_pipeline(renderer.context);
        Framebuffer::update_size(renderer.framebuffer, renderer.debug, renderer.context);
        RenderGlobalBuffer::setup_initial_pipeline(renderer.globalbuffer,renderer.context);
        TextureHolder::setup_initial_pipeline(renderer.textureholder, renderer.context);
        Log::info(LOG_CAT, "Setup initial pipeline.");
    }
    // ===================
}

void Renderer::begin_draw(const RendererState& renderer)
{
    Framebuffer::clear(renderer.framebuffer, renderer.context);
}

void Renderer::draw_static_mesh(const RendererState& renderer, const Mat4 view_proj_mat, const StaticMeshDrawData& mesh_data, const Mat4 world_transform)
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
            renderer.debug,
            renderer.context.context->Map(
                renderer.globalbuffer.constant_buffer.get(),
                0,
                D3D11_MAP_WRITE_DISCARD,
                0,
                &drawable_data_resource
            )
        );
        memcpy(drawable_data_resource.pData, &drawable_data, sizeof(drawable_data));
        renderer.context.context->Unmap(renderer.globalbuffer.constant_buffer.get(), 0);
    }
    // ===================

    // Shader
    {
        const ShaderHolder::ShaderState& shader = ShaderHolder::get_shader_state(renderer.shaderholder, (uint8_t)mesh_data.shader_type);
        renderer.context.context->IASetInputLayout(shader.input_layout.get());
        renderer.context.context->VSSetShader(shader.vertex.get(), nullptr, 0);
        renderer.context.context->PSSetShader(shader.pixel.get(), nullptr, 0);
    }
    // ===================

    // Texture
    {
        if (!mesh_data.texture_id.is_nil())
        {
            const TextureHolder::GpuTextureData& texture_state = renderer.textureholder.textures.at(mesh_data.texture_id);
            renderer.context.context->PSSetShaderResources(0, 1, texture_state.texture_view.addressof());
        }
        else
        {
            ID3D11ShaderResourceView* null_view = nullptr;
            renderer.context.context->PSSetShaderResources(0, 1, &null_view);
        }
    }
    // ===================

    // Mesh/Draw
    {
        const MeshHolder::GpuMeshData& mesh_state = renderer.meshholder.static_meshes.at(mesh_data.mesh_id); // TODO: Batch them
        renderer.context.context->IASetVertexBuffers(
            0,
            (uint32_t)mesh_state.vertex_buffers.size(),
            mesh_state.vertex_buffers.data()->addressof(),
            mesh_state.strides.data(),
            Common::NO_OFFSETS.data()
        );
        renderer.context.context->IASetIndexBuffer(mesh_state.index_buffer.get(), DXGI_FORMAT_R32_UINT, 0);
        renderer.context.context->DrawIndexed(mesh_state.index_count, 0, 0);
    }
    // ===================
}

void Renderer::end_draw(const RendererState& renderer)
{
    Framebuffer::present(renderer.framebuffer);
}

uuids::uuid Renderer::upload_texture(RendererState& renderer, std::string_view texture_file_name)
{
    return TextureHolder::upload_texture(renderer.textureholder, renderer.debug, renderer.context, texture_file_name);
}

uuids::uuid Renderer::upload_mesh(RendererState& renderer, const char* file_path)
{
    return MeshHolder::upload_mesh(renderer.meshholder, renderer.debug, renderer.context, file_path);
}

void Renderer::on_window_size_change(RendererState& renderer)
{
    Framebuffer::update_size(renderer.framebuffer, renderer.debug, renderer.context);
}
