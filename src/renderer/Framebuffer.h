//
// Created by TheAwesomeGem on 11/29/2025.
//

#pragma once
#include <d3d11.h>
#include <dxgi1_5.h>

#include "Adapter.h"
#include "RenderContext.h"
#include "src/Common.h"

namespace Framebuffer
{
    struct BufferState
    {
        NOT_COPYABLE_AND_MOVEABLE(BufferState);
        BufferState() = default;

        com_ptr<IDXGISwapChain4> swap_chain = {};
        com_ptr<ID3D11RenderTargetView> color_buffer_view = {};
        com_ptr<ID3D11DepthStencilView> depth_buffer_view = {};
        Vec2I size = Vec2I{ .x = 0, .y = 0 };
    };

    void create(
        BufferState& buffer,
        const RenderDebug::DebugState& debug,
        const Adapter::AdapterState& adapter,
        const RenderContext::ContextState& context,
        HWND window_handle
    );
    void update_size(BufferState& buffer, const RenderDebug::DebugState& debug, const RenderContext::ContextState& context);
    void clear(const BufferState& buffer, const RenderContext::ContextState& context);
    void present(const BufferState& buffer);

}
