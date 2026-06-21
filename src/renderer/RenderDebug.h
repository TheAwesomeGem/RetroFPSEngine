//
// Created by TheAwesomeGem on 11/29/2025.
//

#pragma once
#include <d3dcommon.h>
#include <dxgidebug.h>

#include "src/Common.h"


namespace RenderDebug
{
    struct DebugState
    {
        NOT_COPYABLE_AND_MOVEABLE(DebugState);
        DebugState() = default;

        com_ptr<IDXGIInfoQueue> debug_info = {};
    };

    void create(DebugState& debug);
    void process_debug_messages(const DebugState& debug);
    void check(const DebugState& debug, HRESULT hr);
    void sh_check(const DebugState& debug, HRESULT res, ID3DBlob* error_message);

}
