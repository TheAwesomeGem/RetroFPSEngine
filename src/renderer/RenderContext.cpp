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

RenderContext::RenderContext(RenderDebug* debug, Adapter* adapter)
    : m_debug{ debug }, m_adapter{ adapter }
{
}

void RenderContext::init()
{
    constexpr auto requested_feature_levels = std::to_array<D3D_FEATURE_LEVEL>({ D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 });
    com_ptr<ID3D11Device> device;
    com_ptr<ID3D11DeviceContext> context;
    m_debug->check(
        D3D11CreateDevice(
            m_adapter->m_gpu.get(),
            D3D_DRIVER_TYPE_UNKNOWN,
            nullptr, // only for software rendering
            D3D11_CREATE_DEVICE_DEBUG,
            requested_feature_levels.data(),
            // ReSharper disable once CppRedundantCastExpression
            (uint32_t)std::size(requested_feature_levels),
            D3D11_SDK_VERSION, device.put(), nullptr, context.put()
        )
    );

    if (!device || !context)
    {
        Log::fatal(LOG_CAT, "Failed creating device and context.");

        return;
    }

    m_debug->check(device->QueryInterface(IID_PPV_ARGS(m_device.put())));

    if (!m_device)
    {
        Log::fatal(LOG_CAT, "Failed getting device v2.");

        return;
    }

    m_debug->check(context->QueryInterface(IID_PPV_ARGS(m_context.put())));

    if (!m_context)
    {
        Log::fatal(LOG_CAT, "Failed getting context v2.");

        // ReSharper disable once CppRedundantControlFlowJump
        return;
    }
}

void RenderContext::setup_initial_pipeline()
{
    m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
