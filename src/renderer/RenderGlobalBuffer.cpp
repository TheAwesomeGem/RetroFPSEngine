//
// Created by TheAwesomeGem on 11/30/2025.
//

#include "RenderGlobalBuffer.h"

#include "RenderContext.h"
#include "RenderDebug.h"
#include "RendererCommon.h"
#include "src/Log.h"

using RendererCommon::LOG_CAT;

void RenderGlobalBuffer::create(BufferState& buffer, const RenderDebug::DebugState& debug, const RenderContext::ContextState& context)
{
    D3D11_BUFFER_DESC buffer_desc = {};
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    buffer_desc.ByteWidth = sizeof(GlobalData);
    buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    RenderDebug::check(debug,
        context.device->CreateBuffer(&buffer_desc, nullptr, buffer.constant_buffer.put())
    );

    if (!buffer.constant_buffer)
    {
        Log::fatal(LOG_CAT, "Failed to create global constant buffer.");

        return;
    }
}

void RenderGlobalBuffer::setup_initial_pipeline(BufferState& buffer, const RenderContext::ContextState& context)
{
    context.context->VSSetConstantBuffers(0, 1, buffer.constant_buffer.addressof());
    context.context->PSSetConstantBuffers(0, 1, buffer.constant_buffer.addressof());
}
