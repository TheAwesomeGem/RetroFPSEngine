//
// Created by TheAwesomeGem on 11/30/2025.
//

#pragma once
#include <d3d11.h>
#include <uuid.h>
#include <vector>

#include "src/Common.h"

class RenderContext;
class RenderDebug;

struct GpuTextureData
{
    com_ptr<ID3D11ShaderResourceView> texture_view = nullptr;
};

class TextureHolder
{
public:
    NOT_COPYABLE_AND_MOVEABLE(TextureHolder);

    TextureHolder(RenderDebug* debug, RenderContext* context);
    void init();
    void setup_initial_pipeline();
    uuids::uuid upload_texture(std::string_view texture_file_name);

    [[nodiscard]] const GpuTextureData& get_texture_data(uuids::uuid resource_id) const
    {
        return m_textures.at(resource_id);
    }

    com_ptr<ID3D11SamplerState> m_sampler;

private:

    std::unordered_map<uuids::uuid, GpuTextureData> m_textures;
    RenderDebug* m_debug;
    RenderContext* m_context;
};
