//
// Created by TheAwesomeGem on 11/30/2025.
//

#include "RenderGlobalBuffer.h"

#include "RenderContext.h"
#include "RenderDebug.h"
#include "RendererCommon.h"
#include "src/Log.h"

using RendererCommon::LOG_CAT;

RenderGlobalBuffer::RenderGlobalBuffer(RenderDebug* debug, RenderContext* context)
    : m_debug{ debug }, m_context{ context }
{
}

void RenderGlobalBuffer::init()
{
    D3D11_BUFFER_DESC buffer_desc = {};
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    buffer_desc.ByteWidth = sizeof(GlobalData);
    buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    m_debug->check(
        m_context->m_device->CreateBuffer(&buffer_desc, nullptr, m_constant_buffer.put())
    );

    if (!m_constant_buffer)
    {
        Log::fatal(LOG_CAT, "Failed to create global constant buffer.");

        return;
    }
}

void RenderGlobalBuffer::setup_initial_pipeline()
{
    m_context->m_context->VSSetConstantBuffers(0, 1, m_constant_buffer.addressof());
    m_context->m_context->PSSetConstantBuffers(0, 1, m_constant_buffer.addressof());
}
