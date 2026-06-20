//
// Created by TheAwesomeGem on 11/29/2025.
//

#pragma once
#include <d3d11.h>
#include <dxgi1_5.h>

#include "Adapter.h"
#include "src/Common.h"


class RenderContext;
class Adapter;
class RenderDebug;

class Framebuffer
{
public:
    NOT_COPYABLE_AND_MOVEABLE(Framebuffer);

    Framebuffer(RenderDebug* debug, Adapter* adapter, RenderContext* context);
    void init(HWND window_handle);
    void update_size();
    void clear() const;
    void present() const;
    [[nodiscard]] Vec2I get_size() const
    {
        return m_size;
    }

    com_ptr<IDXGISwapChain4> m_swap_chain;
    com_ptr<ID3D11RenderTargetView> m_color_buffer_view;
    com_ptr<ID3D11DepthStencilView> m_depth_buffer_view;
private:
    Vec2I m_size;
    RenderDebug* m_debug;
    Adapter* m_adapter;
    RenderContext* m_context;
};
