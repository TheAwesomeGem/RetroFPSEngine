//
// Created by TheAwesomeGem on 11/29/2025.
//

#pragma once
#include <d3dcommon.h>
#include <dxgidebug.h>

#include "src/Common.h"


class RenderDebug
{
public:
    NOT_COPYABLE_AND_MOVEABLE(RenderDebug);

    RenderDebug() = default;
    void init();
    void process_debug_messages() const;
    void check(HRESULT hr) const;
    void sh_check(HRESULT res, ID3DBlob* error_message) const;

    com_ptr<IDXGIInfoQueue> m_debug_info;
};
