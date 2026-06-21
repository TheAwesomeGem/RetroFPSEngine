//
// Created by TheAwesomeGem on 11/30/2025.
//

#pragma once
#include <d3d11.h>
#include <vector>
#include <stduuid/uuid.h>

#include "ShaderHolder.h"
#include "src/Common.h"

namespace MeshHolder
{
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

    struct HolderState
    {
        NOT_COPYABLE_AND_MOVEABLE(HolderState);
        HolderState() = default;

        std::unordered_map<uuids::uuid, GpuMeshData> static_meshes = {};
    };

    void create();
    uuids::uuid upload_mesh(HolderState& holder, const RenderDebug::DebugState& debug, const RenderContext::ContextState& context, const char* file_path);
}
