//
// Created by TheAwesomeGem on 11/29/2025.
//

#include "Adapter.h"

#include "RenderDebug.h"
#include "RendererCommon.h"
#include "src/Log.h"
#include "src/util/StringExt.h"

using RendererCommon::LOG_CAT;

Adapter::Adapter(RenderDebug* debug)
    : m_debug{ debug }
{
}

void Adapter::init()
{
    m_debug->check(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(m_factory.put())));

    if (!m_factory)
    {
        Log::fatal(LOG_CAT, "Failed creating dxgi factory.");

        return;
    }

    com_ptr<IDXGIAdapter1> old_adapter;
    m_debug->check(m_factory->EnumAdapters1(0, old_adapter.put()));

    if (!old_adapter)
    {
        Log::fatal(LOG_CAT, "Failed fetching video adapter.");

        return;
    }

    m_debug->check(old_adapter->QueryInterface(IID_PPV_ARGS(m_gpu.put())));

    if (!m_gpu)
    {
        Log::fatal(LOG_CAT, "Failed fetching video adapter v4.");

        // ReSharper disable once CppRedundantControlFlowJump
        return;
    }

    DXGI_ADAPTER_DESC3 adapter_desc;
    m_debug->check(m_gpu->GetDesc3(&adapter_desc));

    Log::info(
        LOG_CAT, "Active GPU is {}", StringUtils::wide_to_utf8(adapter_desc.Description)
    );
}

const char* Adapter::get_feature_level_str(D3D_FEATURE_LEVEL feature_level)
{
    switch (feature_level)
    {
        case D3D_FEATURE_LEVEL_11_0:
            return "11.0";
        case D3D_FEATURE_LEVEL_11_1:
            return "11.1";
        default:
            return "Unsupported";
    }
}
