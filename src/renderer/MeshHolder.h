//
// Created by TheAwesomeGem on 11/30/2025.
//

#pragma once
#include <d3d11.h>
#include <vector>
#include <stduuid/uuid.h>

#include "ShaderHolder.h"
#include "src/Common.h"

class ShaderHolder;
class RenderContext;
class RenderDebug;

struct VertexAttrib
{
    com_ptr<ID3D11Buffer> buffer = {};
    uint32_t stride = 0;
    uint32_t element_count = 0;
};

struct GpuMeshData
{
    std::vector<com_ptr<ID3D11Buffer>> vertex_buffers;
    std::vector<uint32_t> strides;
    com_ptr<ID3D11Buffer> index_buffer = nullptr;
    uint32_t index_count = 0; // The number of vertices
};

struct CpuMeshData
{
    std::vector<Vec3F> pos = {};
    std::vector<Vec3F> normals = {};
    std::vector<Vec2F> uv = {};
    std::vector<uint32_t> indices = {};
};

class MeshHolder
{
public:
    NOT_COPYABLE_AND_MOVEABLE(MeshHolder);

    MeshHolder(RenderDebug* debug, RenderContext* context);
    void init();
    uuids::uuid upload_mesh(const char* file_path);

    [[nodiscard]] const GpuMeshData& get_static_mesh(uuids::uuid resource_id)
    {
        return m_static_meshes.at(resource_id);
    }

private:
    VertexAttrib create_vertex_attrib(const void* data, uint32_t element_count, uint32_t stride) const;

    std::unordered_map<uuids::uuid, GpuMeshData> m_static_meshes;
    RenderDebug* m_debug;
    RenderContext* m_context;
};
