//
// Created by TheAwesomeGem on 11/29/2025.
//

#pragma once
#include <d3dcommon.h>
#include <dxgi1_6.h>

#include "RenderDebug.h"
#include "src/Common.h"

namespace Adapter
{
    struct AdapterState
    {
        NOT_COPYABLE_AND_MOVEABLE(AdapterState);
        AdapterState() = default;

        com_ptr<IDXGIFactory7> factory = {};
        com_ptr<IDXGIAdapter4> gpu = {};
    };

    void create(AdapterState& adapter, const RenderDebug::DebugState& debug);
}
