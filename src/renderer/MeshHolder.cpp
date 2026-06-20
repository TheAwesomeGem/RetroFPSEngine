//
// Created by TheAwesomeGem on 11/30/2025.
//

#include "MeshHolder.h"

#include <print>

#include "MeshBuilder.h"
#include "MeshLoader.h"
#include "RenderDebug.h"
#include "RendererCommon.h"
#include "ShaderHolder.h"
#include "src/Log.h"
#include "src/util/RandomExt.h"

using RendererCommon::LOG_CAT;


MeshHolder::MeshHolder(RenderDebug* debug, RenderContext* context)
    : m_debug{ debug }, m_context{ context }, m_static_meshes{}
{
}

void MeshHolder::init()
{
}

uuids::uuid MeshHolder::upload_mesh(const char* file_path)
{
    CpuMeshData cpu_mesh = MeshLoader::load_mesh(file_path);

    GpuMeshData gpu_mesh = {};
    gpu_mesh.index_count = (int)std::size(cpu_mesh.indices);

    VertexAttrib attrib = create_vertex_attrib(
        cpu_mesh.pos.data(),
        (uint32_t)cpu_mesh.pos.size(),
        sizeof(*cpu_mesh.pos.data())
    );
    gpu_mesh.vertex_buffers.emplace_back(attrib.buffer);
    gpu_mesh.strides.emplace_back(attrib.stride);

    attrib = create_vertex_attrib(
        cpu_mesh.normals.data(),
        (uint32_t)cpu_mesh.normals.size(),
        sizeof(*cpu_mesh.normals.data())
    );
    gpu_mesh.vertex_buffers.emplace_back(attrib.buffer);
    gpu_mesh.strides.emplace_back(attrib.stride);

    attrib = create_vertex_attrib(
        cpu_mesh.uv.data(),
        (uint32_t)cpu_mesh.uv.size(),
        sizeof(*cpu_mesh.uv.data())
    );
    gpu_mesh.vertex_buffers.emplace_back(attrib.buffer);
    gpu_mesh.strides.emplace_back(attrib.stride);

    D3D11_BUFFER_DESC buffer_desc = {};
    buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    buffer_desc.ByteWidth = (uint32_t)(cpu_mesh.indices.size() * sizeof(*cpu_mesh.indices.data()));
    buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
    const D3D11_SUBRESOURCE_DATA data = D3D11_SUBRESOURCE_DATA{
        .pSysMem = cpu_mesh.indices.data(), .SysMemPitch = 0, .SysMemSlicePitch = 0
    };
    m_debug->check(m_context->m_device->CreateBuffer(&buffer_desc, &data, gpu_mesh.index_buffer.put()));

    if (!gpu_mesh.index_buffer)
    {
        Log::fatal(LOG_CAT, "Failed to create index buffer for mesh.");

        return uuids::uuid{};
    }

    uuids::uuid id = RandomExt::id();

    m_static_meshes.emplace(id, std::move(gpu_mesh));

    return id;
}


VertexAttrib MeshHolder::create_vertex_attrib(const void* data, uint32_t element_count, uint32_t stride) const
{
    D3D11_BUFFER_DESC buffer_desc = {};
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.ByteWidth = element_count * stride;
    buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
    const D3D11_SUBRESOURCE_DATA sub_data = D3D11_SUBRESOURCE_DATA{ .pSysMem = data, .SysMemPitch = 0, .SysMemSlicePitch = 0 };
    com_ptr<ID3D11Buffer> vertex_buffer;
    m_debug->check(m_context->m_device->CreateBuffer(&buffer_desc, &sub_data, vertex_buffer.put()));

    if (!vertex_buffer)
    {
        Log::fatal(LOG_CAT, "Failed to create vertex buffer for mesh.");

        return VertexAttrib{};
    }

    return VertexAttrib{
        .buffer = vertex_buffer,
        .stride = stride,
        .element_count = element_count
    };
}
