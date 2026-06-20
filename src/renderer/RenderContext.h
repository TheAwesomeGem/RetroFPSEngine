//
// Created by TheAwesomeGem on 11/29/2025.
//

#pragma once
#include <d3d11_2.h>

#include "src/Common.h"


class Adapter;
class RenderDebug;

class RenderContext
{
public:
    NOT_COPYABLE_AND_MOVEABLE(RenderContext);

    explicit RenderContext(RenderDebug* debug, Adapter* adapter);
    void init();
    void setup_initial_pipeline();

    com_ptr<ID3D11Device2> m_device;
    com_ptr<ID3D11DeviceContext2> m_context;
private:
    RenderDebug* m_debug;
    Adapter* m_adapter;
};
