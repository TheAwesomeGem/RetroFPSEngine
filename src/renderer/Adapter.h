//
// Created by TheAwesomeGem on 11/29/2025.
//

#pragma once
#include <d3dcommon.h>
#include <dxgi1_6.h>

#include "src/Common.h"


class RenderDebug;

class Adapter
{
public:
    NOT_COPYABLE_AND_MOVEABLE(Adapter);

    explicit Adapter(RenderDebug* debug);
    void init();

    com_ptr<IDXGIFactory7> m_factory;
    com_ptr<IDXGIAdapter4> m_gpu;

private:
    static const char* get_feature_level_str(D3D_FEATURE_LEVEL feature_level);

    RenderDebug* m_debug;
};
