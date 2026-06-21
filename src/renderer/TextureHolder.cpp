//
// Created by TheAwesomeGem on 11/30/2025.
//

#include "TextureHolder.h"

#include <WICTextureLoader.h>

#include "RenderContext.h"
#include "RenderDebug.h"
#include "RendererCommon.h"
#include "src/Log.h"
#include "src/util/RandomExt.h"
#include "src/util/StringExt.h"

using RendererCommon::LOG_CAT;

void TextureHolder::create(HolderState& holder, const RenderDebug::DebugState& debug, const RenderContext::ContextState& context)
{
    // init samplers
    {
        D3D11_SAMPLER_DESC sampler_state_desc = {};
        sampler_state_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_state_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_state_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_state_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        sampler_state_desc.MaxAnisotropy = 1;
        RenderDebug::check(debug,
            context.device->CreateSamplerState(&sampler_state_desc, holder.sampler.put())
        );

        if (holder.sampler == nullptr)
        {
            Log::fatal(LOG_CAT, "Failed to create basic texture sampler.");

            return;
        }
    }
    // =================
}

void TextureHolder::setup_initial_pipeline(const HolderState& holder, const RenderContext::ContextState& context)
{
    context.context->PSSetSamplers(0, 1, holder.sampler.addressof());
}

uuids::uuid TextureHolder::upload_texture(HolderState& holder, const RenderDebug::DebugState& debug, const RenderContext::ContextState& context, std::string_view texture_file_name)
{
    com_ptr<ID3D11Resource> texture;
    com_ptr<ID3D11ShaderResourceView> texture_view;
    // NOTE: This generates 10x of the file size of a png as the memory required to load. So 100kb file = 1mb of memory required in CPU
    RenderDebug::check(debug,
        DirectX::CreateWICTextureFromFile(
            context.device.get(), StringUtils::utf8_to_wide(texture_file_name).data(), texture.put(), texture_view.put()
        )
    );

    if (!texture_view)
    {
        Log::fatal(LOG_CAT, "Failed to create texture SRV.");

        return uuids::uuid{};
    }

    uuids::uuid id = RandomExt::id();

    holder.textures.emplace(id, GpuTextureData{ .texture_view = texture_view });

    return id;
}
