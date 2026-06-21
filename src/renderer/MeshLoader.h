//
// Created by TheAwesomeGem on 12/15/2025.
//

#pragma once
#include <filesystem>

#include "MeshHolder.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace MeshLoader
{
    inline MeshHolder::CpuMeshData load_mesh(const char* file_path)
    {
        assert(std::filesystem::exists(file_path));

        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(
            file_path,
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_GenNormals
        );

        assert(scene->HasMeshes());

        aiMesh* mesh = scene->mMeshes[0];
        MeshHolder::CpuMeshData mesh_data = {};

        for (size_t i = 0; i < mesh->mNumVertices; ++i)
        {
            aiVector3D vertex = mesh->mVertices[i];
            mesh_data.pos.emplace_back(vertex.x, vertex.y, vertex.z);

            if (mesh->HasNormals())
            {
                aiVector3D normal = mesh->mNormals[i];
                mesh_data.normals.emplace_back(normal.x, normal.y, normal.z);
            }

            if (mesh->HasTextureCoords(0)) // 0 is the first texture coordinate channel
            {
                aiVector3D tex_coord = mesh->mTextureCoords[0][i];
                mesh_data.uv.emplace_back(tex_coord.x, tex_coord.y);
            }
            else
            {
                // If no texture coordinates are available, use (0, 0) as a default
                mesh_data.uv.emplace_back(0.0f, 0.0f);
            }
        }

        for (size_t i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];

            for (size_t j = 0; j < face.mNumIndices; ++j)
            {
                mesh_data.indices.emplace_back(face.mIndices[j]);
            }
        }

        return mesh_data;
    }
}
