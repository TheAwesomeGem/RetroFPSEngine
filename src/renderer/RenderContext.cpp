//
// Created by TheAwesomeGem on 11/29/2025.
//

#include "RenderContext.h"

#include <array>

#include "Adapter.h"
#include "RenderDebug.h"
#include "RendererCommon.h"
#include "src/Log.h"

using RendererCommon::LOG_CAT;

void RenderContext::create(RenderContext::ContextState& context, const RenderDebug::DebugState& debug, const Adapter::AdapterState& adapter)
{
    constexpr auto requested_feature_levels = std::to_array<D3D_FEATURE_LEVEL>({ D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 });
    com_ptr<ID3D11Device> device;
    com_ptr<ID3D11DeviceContext> device_context;
    RenderDebug::check(
        debug,
        D3D11CreateDevice(
            adapter.gpu.get(),
            D3D_DRIVER_TYPE_UNKNOWN,
            nullptr, // only for software rendering
            D3D11_CREATE_DEVICE_DEBUG,
            requested_feature_levels.data(),
            // ReSharper disable once CppRedundantCastExpression
            (uint32_t)std::size(requested_feature_levels),
            D3D11_SDK_VERSION, device.put(), nullptr, device_context.put()
        )
    );

    if (!device || !device_context)
    {
        Log::fatal(LOG_CAT, "Failed creating device and context.");

        return;
    }

    RenderDebug::check(debug,device->QueryInterface(IID_PPV_ARGS(context.device.put())));

    if (!context.device)
    {
        Log::fatal(LOG_CAT, "Failed getting device v2.");

        return;
    }

    RenderDebug::check(debug,device_context->QueryInterface(IID_PPV_ARGS(context.context.put())));

    if (!context.context)
    {
        Log::fatal(LOG_CAT, "Failed getting context v2.");

        // ReSharper disable once CppRedundantControlFlowJump
        return;
    }
}

void RenderContext::setup_initial_pipeline(const RenderContext::ContextState& context)
{
    context.context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
