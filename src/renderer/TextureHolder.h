//
// Created by TheAwesomeGem on 11/30/2025.
//

#pragma once
#include <d3d11.h>
#include <uuid.h>
#include <vector>

#include "RenderContext.h"
#include "RenderDebug.h"
#include "src/Common.h"

namespace TextureHolder
{
    struct GpuTextureData
    {
        com_ptr<ID3D11ShaderResourceView> texture_view = nullptr;
    };

    struct HolderState
    {
        NOT_COPYABLE_AND_MOVEABLE(HolderState);
        HolderState() = default;

        com_ptr<ID3D11SamplerState> sampler = {};
        std::unordered_map<uuids::uuid, GpuTextureData> textures = {};
    };

    void create(HolderState& holder, const RenderDebug::DebugState& debug, const RenderContext::ContextState& context);
    void setup_initial_pipeline(const HolderState& holder, const RenderContext::ContextState& context);
    uuids::uuid upload_texture(HolderState& holder, const RenderDebug::DebugState& debug, const RenderContext::ContextState& context, std::string_view texture_file_name);
}
