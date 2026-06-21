//
// Created by TheAwesomeGem on 11/30/2025.
//

#pragma once
#include <array>
#include <d3d11.h>

#include "RenderContext.h"
#include "RenderDebug.h"
#include "src/Common.h"

namespace RenderGlobalBuffer
{
    struct GlobalData
    {
        Mat4 pos_transform = Mat4::Identity;
        Mat4 normal_transform = Mat4::Identity;
        Vec4F tint_color = { 1.0F, 1.0F, 1.0F, 1.0F };
    };

    struct BufferState
    {
        NOT_COPYABLE_AND_MOVEABLE(BufferState);
        BufferState() = default;

        com_ptr<ID3D11Buffer> constant_buffer = {}; // TODO: For now, this is just for world transform
    };

    void create(BufferState& buffer, const RenderDebug::DebugState& debug, const RenderContext::ContextState& context);
    void setup_initial_pipeline(BufferState& buffer, const RenderContext::ContextState& context);
}
