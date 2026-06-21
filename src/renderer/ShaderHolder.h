//
// Created by TheAwesomeGem on 11/29/2025.
//

#pragma once
#include <d3d11.h>
#include <deque>
#include <unordered_map>
#include <vector>

#include "RenderContext.h"

namespace ShaderHolder
{
    enum class ShaderInputElementType: uint8_t
    {
        pos = 0,
        normal,
        uv,
        count
    };

    enum class ShaderRenderType: uint8_t
    {
        textured = 0,
        colored,
        count
    };

    struct ShaderState
    {
        com_ptr<ID3D11VertexShader> vertex = nullptr;
        com_ptr<ID3D11PixelShader> pixel = nullptr;
        com_ptr<ID3DBlob> vertex_bytecode = nullptr;
        com_ptr<ID3D11InputLayout> input_layout = nullptr;
    };

    struct HolderState
    {
        NOT_COPYABLE_AND_MOVEABLE(HolderState);
        HolderState() = default;

        std::vector<ShaderState> shaders = {};
    };

    void create(HolderState& holder, const RenderDebug::DebugState& debug, const RenderContext::ContextState& context);
    [[nodiscard]] inline const ShaderState& get_shader_state(const HolderState& holder, size_t idx)
    {
        return holder.shaders[idx];
    }
}
