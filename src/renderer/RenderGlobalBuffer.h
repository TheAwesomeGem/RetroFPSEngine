//
// Created by TheAwesomeGem on 11/30/2025.
//

#pragma once
#include <array>
#include <d3d11.h>

#include "src/Common.h"

class RenderContext;
class RenderDebug;

struct GlobalData
{
    Mat4 pos_transform = Mat4::Identity;
    Mat4 normal_transform = Mat4::Identity;
    Vec4F tint_color = { 1.0F, 1.0F, 1.0F, 1.0F };
};

class RenderGlobalBuffer
{
public:
    NOT_COPYABLE_AND_MOVEABLE(RenderGlobalBuffer);

    RenderGlobalBuffer(RenderDebug* debug, RenderContext* context);
    void init();
    void setup_initial_pipeline();

    com_ptr<ID3D11Buffer> m_constant_buffer; // TODO: For now, this is just for world transform

private:
    RenderDebug* m_debug;
    RenderContext* m_context;
};
