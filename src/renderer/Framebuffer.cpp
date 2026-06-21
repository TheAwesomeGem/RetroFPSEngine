//
// Created by TheAwesomeGem on 11/29/2025.
//

#include "Framebuffer.h"

#include <array>

#include "Adapter.h"
#include "RenderContext.h"
#include "RenderDebug.h"
#include "RendererCommon.h"
#include "src/Log.h"

using RendererCommon::LOG_CAT;

void Framebuffer::create(
        BufferState& buffer,
        const RenderDebug::DebugState& debug,
        const Adapter::AdapterState& adapter,
        const RenderContext::ContextState& context,
        HWND window_handle
    )
{
    DXGI_SWAP_CHAIN_DESC1 swap_chain_desc;
    swap_chain_desc.Width = 0; // 0 means DXGI will figure it out from the platform handle
    swap_chain_desc.Height = 0; // 0 means DXGI will figure it out from the platform handle
    swap_chain_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This seems to be the best format due to little endian. UNorm = 0.0 to 1.0
    swap_chain_desc.Stereo = 0; // Stereo is for VR and 3d glasses
    swap_chain_desc.SampleDesc = DXGI_SAMPLE_DESC{
        // This is for MSAA. 1 = 1x super sampling.
        1, 0
    };
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.BufferCount = 1;
    // this is the number of back buffers for windowed or number of back buffers + 1 for full screen(cause it includes front buffer)
    swap_chain_desc.Scaling = DXGI_SCALING_STRETCH;
    // When swap chain size isn't the same as the platform size, it stretches it otherwise it shouldn't do anything.
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // How to switch between all the different buffers(including front buffer)
    swap_chain_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED; // The swap chain alpha mode. Use default for platform.
    swap_chain_desc.Flags = 0;

    com_ptr<IDXGISwapChain1> old_swap_chain;
    RenderDebug::check(debug,
        adapter.factory->CreateSwapChainForHwnd(
            context.device.get(), window_handle, &swap_chain_desc, nullptr,
            // no full screen
            nullptr, // i guess this allows the rendering to only happen on a specific monitor?
            old_swap_chain.put()
        )
    );

    if (!old_swap_chain)
    {
        Log::fatal(LOG_CAT, "Failed creating swap chain v1.");

        return;
    }

    RenderDebug::check(debug, old_swap_chain->QueryInterface(IID_PPV_ARGS(buffer.swap_chain.put())));

    if (!buffer.swap_chain)
    {
        Log::fatal(LOG_CAT, "Failed getting swap chain v4.");

        // ReSharper disable once CppRedundantControlFlowJump
        return;
    }
}

void Framebuffer::update_size(BufferState& buffer, const RenderDebug::DebugState& debug, const RenderContext::ContextState& context)
{
    context.context->OMSetRenderTargets(0, nullptr, nullptr);
    buffer.color_buffer_view.reset();
    buffer.depth_buffer_view.reset();

    RenderDebug::check(debug,buffer.swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0));

    com_ptr<ID3D11Texture2D> color_buffer_texture;
    RenderDebug::check(debug,buffer.swap_chain->GetBuffer(0, IID_PPV_ARGS(color_buffer_texture.put())));

    if (!color_buffer_texture)
    {
        Log::fatal(LOG_CAT, "Failed getting color buffer texture from swap chain.");
        return;
    }

    RenderDebug::check(debug,
        context.device->CreateRenderTargetView(color_buffer_texture.get(), nullptr, buffer.color_buffer_view.put())
    );

    if (!buffer.color_buffer_view)
    {
        Log::fatal(LOG_CAT, "Failed creating color buffer view.");
        return;
    }

    D3D11_TEXTURE2D_DESC color_buffer_texture_desc;
    color_buffer_texture->GetDesc(&color_buffer_texture_desc);

    D3D11_TEXTURE2D_DESC depth_buffer_texture_desc = {};
    depth_buffer_texture_desc.ArraySize = 1;
    depth_buffer_texture_desc.Usage = D3D11_USAGE_DEFAULT;
    depth_buffer_texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depth_buffer_texture_desc.Format = DXGI_FORMAT_D32_FLOAT;
    depth_buffer_texture_desc.Width = color_buffer_texture_desc.Width;
    depth_buffer_texture_desc.Height = color_buffer_texture_desc.Height;
    depth_buffer_texture_desc.SampleDesc.Count = 1;
    depth_buffer_texture_desc.SampleDesc.Quality = 0;
    com_ptr<ID3D11Texture2D> depth_buffer_texture;
    RenderDebug::check(debug,
        context.device->CreateTexture2D(&depth_buffer_texture_desc, nullptr, depth_buffer_texture.put())
    );

    if (!depth_buffer_texture)
    {
        Log::fatal(LOG_CAT, "Failed creating depth buffer texture.");
        return;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC depth_buffer_view_desc = {};
    depth_buffer_view_desc.Format = depth_buffer_texture_desc.Format;
    depth_buffer_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    RenderDebug::check(debug,
        context.device->CreateDepthStencilView(depth_buffer_texture.get(), nullptr, buffer.depth_buffer_view.put())
    );

    if (!buffer.depth_buffer_view)
    {
        Log::fatal(LOG_CAT, "Failed creating depth buffer view.");
        return;
    }

    context.context->OMSetRenderTargets(1, buffer.color_buffer_view.addressof(), buffer.depth_buffer_view.get());

    DXGI_SWAP_CHAIN_DESC1 swap_chain_desc;
    buffer.swap_chain->GetDesc1(&swap_chain_desc);
    const D3D11_VIEWPORT viewport = {
        0.0F, 0.0F, (float)swap_chain_desc.Width, (float)swap_chain_desc.Height, 0.0F, 1.0F
    };
    context.context->RSSetViewports(1, &viewport);

    buffer.size = Vec2I{ .x = (int)swap_chain_desc.Width, .y = (int)swap_chain_desc.Height };
}

void Framebuffer::clear(const BufferState& buffer, const RenderContext::ContextState& context)
{
    context.context->ClearDepthStencilView(buffer.depth_buffer_view.get(), D3D11_CLEAR_DEPTH, 1.0F, 0);
    static constexpr auto col = std::to_array<float>({ 0.025F, 0.075F, 0.125F, 1.0F });
    context.context->ClearRenderTargetView(buffer.color_buffer_view.get(), col.data());
}

void Framebuffer::present(const BufferState& buffer)
{
    buffer.swap_chain->Present(0, 0); // 0 = no vsync, > 0 is Vsync n-frames
}
