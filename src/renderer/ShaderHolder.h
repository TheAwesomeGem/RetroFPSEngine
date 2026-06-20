//
// Created by TheAwesomeGem on 11/29/2025.
//

#pragma once
#include <d3d11.h>
#include <unordered_map>
#include <vector>

#include "RenderContext.h"


class RenderContext;
class RenderDebug;

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

class ShaderHolder
{
public:
    NOT_COPYABLE_AND_MOVEABLE(ShaderHolder);

    ShaderHolder(RenderDebug* debug, RenderContext* context);
    void init();

    [[nodiscard]] const ShaderState& get_shader_state(size_t idx) const
    {
        return m_shaders[idx];
    }

private:
    static std::vector<ShaderInputElementType> get_shader_input_element_types(ShaderRenderType shader_render_type);
    static std::vector<D3D11_INPUT_ELEMENT_DESC> get_shader_input_descriptions(const std::vector<ShaderInputElementType>& element_types);

    static std::pair<const char*, const char*> get_shader_file_names(ShaderRenderType shader_render);

    std::vector<ShaderState> m_shaders;
    RenderDebug* m_debug;
    RenderContext* m_context;
};
