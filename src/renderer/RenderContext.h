//
// Created by TheAwesomeGem on 11/29/2025.
//

#pragma once
#include <d3d11_2.h>

#include "Adapter.h"
#include "src/Common.h"

namespace RenderContext
{
    struct ContextState
    {
        NOT_COPYABLE_AND_MOVEABLE(ContextState);
        ContextState() = default;

        com_ptr<ID3D11Device2> device = {};
        com_ptr<ID3D11DeviceContext2> context = {};
    };

    void create(ContextState& context, const RenderDebug::DebugState& debug, const Adapter::AdapterState& adapter);
    void setup_initial_pipeline(const ContextState& context);
}

// class RenderContext
// {
// public:
//     NOT_COPYABLE_AND_MOVEABLE(RenderContext);
//
//     explicit RenderContext(const RenderDebug::DebugState& debug, const Adapter::AdapterState& adapter);
//     void init();
//     void setup_initial_pipeline();
//
//     com_ptr<ID3D11Device2> m_device;
//     com_ptr<ID3D11DeviceContext2> m_context;
// private:
//     const RenderDebug::DebugState* m_debug;
//     const Adapter::AdapterState* m_adapter;
// };
