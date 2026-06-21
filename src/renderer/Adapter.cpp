//
// Created by TheAwesomeGem on 11/29/2025.
//

#include "Adapter.h"

#include "RenderDebug.h"
#include "RendererCommon.h"
#include "src/Log.h"
#include "src/util/StringExt.h"

using RendererCommon::LOG_CAT;

void Adapter::create(AdapterState& adapter, const RenderDebug::DebugState& debug)
{
    RenderDebug::check(debug, CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(adapter.factory.put())));

    if (!adapter.factory)
    {
        Log::fatal(LOG_CAT, "Failed creating dxgi factory.");

        return;
    }

    com_ptr<IDXGIAdapter1> old_adapter;
    RenderDebug::check(debug, adapter.factory->EnumAdapters1(0, old_adapter.put()));

    if (!old_adapter)
    {
        Log::fatal(LOG_CAT, "Failed fetching video adapter.");

        return;
    }

    RenderDebug::check(debug, old_adapter->QueryInterface(IID_PPV_ARGS(adapter.gpu.put())));

    if (!adapter.gpu)
    {
        Log::fatal(LOG_CAT, "Failed fetching video adapter v4.");

        // ReSharper disable once CppRedundantControlFlowJump
        return;
    }

    DXGI_ADAPTER_DESC3 adapter_desc;
    RenderDebug::check(debug, adapter.gpu->GetDesc3(&adapter_desc));

    Log::info(
        LOG_CAT, "Active GPU is {}", StringUtils::wide_to_utf8(adapter_desc.Description)
    );
}
