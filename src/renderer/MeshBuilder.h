//
// Created by TheAwesomeGem on 12/10/2025.
//

#pragma once
#include "src/renderer/MeshHolder.h"

namespace MeshBuilder
{
    inline MeshHolder::CpuMeshData make_cube_mesh()
    {
        MeshHolder::CpuMeshData cube_mesh;

        // Front face
        cube_mesh.pos.emplace_back(-1.0F, -1.0F, 1.0F);
        cube_mesh.pos.emplace_back(-1.0F, 1.0F, 1.0F);
        cube_mesh.pos.emplace_back(1.0F, 1.0F, 1.0F);
        cube_mesh.pos.emplace_back(1.0F, -1.0F, 1.0F);

        // Back face
        cube_mesh.pos.emplace_back(-1.0F, -1.0F, -1.0F);
        cube_mesh.pos.emplace_back(-1.0F, 1.0F, -1.0F);
        cube_mesh.pos.emplace_back(1.0F, 1.0F, -1.0F);
        cube_mesh.pos.emplace_back(1.0F, -1.0F, -1.0F);

        // Left face
        cube_mesh.pos.emplace_back(-1.0F, -1.0F, -1.0F);
        cube_mesh.pos.emplace_back(-1.0F, 1.0F, -1.0F);
        cube_mesh.pos.emplace_back(-1.0F, 1.0F, 1.0F);
        cube_mesh.pos.emplace_back(-1.0F, -1.0F, 1.0F);

        // Right face
        cube_mesh.pos.emplace_back(1.0F, -1.0F, -1.0F);
        cube_mesh.pos.emplace_back(1.0F, 1.0F, -1.0F);
        cube_mesh.pos.emplace_back(1.0F, 1.0F, 1.0F);
        cube_mesh.pos.emplace_back(1.0F, -1.0F, 1.0F);

        // Top face
        cube_mesh.pos.emplace_back(-1.0F, 1.0F, 1.0F);
        cube_mesh.pos.emplace_back(-1.0F, 1.0F, -1.0F);
        cube_mesh.pos.emplace_back(1.0F, 1.0F, -1.0F);
        cube_mesh.pos.emplace_back(1.0F, 1.0F, 1.0F);

        // Bottom face
        cube_mesh.pos.emplace_back(-1.0F, -1.0F, 1.0F);
        cube_mesh.pos.emplace_back(-1.0F, -1.0F, -1.0F);
        cube_mesh.pos.emplace_back(1.0F, -1.0F, -1.0F);
        cube_mesh.pos.emplace_back(1.0F, -1.0F, 1.0F);


        cube_mesh.uv.emplace_back(0.0F, 0.0F);
        cube_mesh.uv.emplace_back(0.0F, 1.0F);
        cube_mesh.uv.emplace_back(1.0F, 1.0F);
        cube_mesh.uv.emplace_back(1.0F, 0.0F);

        cube_mesh.uv.emplace_back(1.0F, 0.0F);
        cube_mesh.uv.emplace_back(1.0F, 1.0F);
        cube_mesh.uv.emplace_back(0.0F, 1.0F);
        cube_mesh.uv.emplace_back(0.0F, 0.0F);

        cube_mesh.uv.emplace_back(0.0F, 0.0F);
        cube_mesh.uv.emplace_back(0.0F, 1.0F);
        cube_mesh.uv.emplace_back(1.0F, 1.0F);
        cube_mesh.uv.emplace_back(1.0F, 0.0F);

        cube_mesh.uv.emplace_back(1.0F, 0.0F);
        cube_mesh.uv.emplace_back(1.0F, 1.0F);
        cube_mesh.uv.emplace_back(0.0F, 1.0F);
        cube_mesh.uv.emplace_back(0.0F, 0.0F);

        cube_mesh.uv.emplace_back(0.0F, 0.0F);
        cube_mesh.uv.emplace_back(0.0F, 1.0F);
        cube_mesh.uv.emplace_back(1.0F, 1.0F);
        cube_mesh.uv.emplace_back(1.0F, 0.0F);

        cube_mesh.uv.emplace_back(1.0F, 0.0F);
        cube_mesh.uv.emplace_back(1.0F, 1.0F);
        cube_mesh.uv.emplace_back(0.0F, 1.0F);
        cube_mesh.uv.emplace_back(0.0F, 0.0F);


        constexpr auto indices = std::to_array<uint32_t>(
            {
                // Front face - CCW
                3, 2, 1, 1, 0, 3,
                // Back face - CW
                4, 5, 6, 6, 7, 4,
                // Left face - CCW
                11, 10, 9, 9, 8, 11,
                // Right face - CW
                12, 13, 14, 14, 15, 12,
                // Top face - CCW
                19, 18, 17, 17, 16, 19,
                // Bottom face - CW
                20, 21, 22, 22, 23, 20,
            }
        );

        cube_mesh.indices = std::vector(indices.begin(), indices.end());

        return cube_mesh;
    }
}
