//
// Created by TheAwesomeGem on 11/29/2025.
//

#include "ShaderHolder.h"

#include <d3dcompiler.h>
#include <filesystem>
#include <ranges>

#include "RenderDebug.h"
#include "RendererCommon.h"
#include "src/Log.h"

using RendererCommon::LOG_CAT;

static std::pair<const char*, const char*> get_shader_file_names(ShaderHolder::ShaderRenderType shader_render)
{
    switch (shader_render)
    {
        case ShaderHolder::ShaderRenderType::textured:
            return std::make_pair("shader/TexturedVertexShader.hlsl", "shader/TexturedPixelShader.hlsl");
        case ShaderHolder::ShaderRenderType::colored:
            return std::make_pair("shader/ColoredVertexShader.hlsl", "shader/ColoredPixelShader.hlsl");
        case ShaderHolder::ShaderRenderType::count:
        default:
            return std::make_pair("shader/BadVertexShader.hlsl", "shader/BadPixelShader.hlsl");
    }
}

static std::vector<ShaderHolder::ShaderInputElementType> get_shader_input_element_types(ShaderHolder::ShaderRenderType shader_render_type)
{
    switch (shader_render_type)
    {
        case ShaderHolder::ShaderRenderType::textured:
        {
            return std::vector{ ShaderHolder::ShaderInputElementType::pos, ShaderHolder::ShaderInputElementType::normal, ShaderHolder::ShaderInputElementType::uv };
        }
        case ShaderHolder::ShaderRenderType::colored:
        {
            return std::vector{ ShaderHolder::ShaderInputElementType::pos, ShaderHolder::ShaderInputElementType::normal };
        }
        case ShaderHolder::ShaderRenderType::count:
        default:
        {
            return std::vector<ShaderHolder::ShaderInputElementType>{};
        }
    }
}

static std::vector<D3D11_INPUT_ELEMENT_DESC> get_shader_input_descriptions(const std::vector<ShaderHolder::ShaderInputElementType>& element_types)
{
    // TODO: Support multiple of the same SemanticName with SemanticIndex possibly?
    std::vector<D3D11_INPUT_ELEMENT_DESC> input_descriptions;
    for (const ShaderHolder::ShaderInputElementType element_type : element_types)
    {
        uint32_t slot = (uint32_t)element_type;

        switch (element_type)
        {
            case ShaderHolder::ShaderInputElementType::pos:
            {
                input_descriptions.emplace_back(
                    "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, slot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0
                );
                break;
            }
            case ShaderHolder::ShaderInputElementType::normal:
            {
                input_descriptions.emplace_back(
                    "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, slot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0
                );
                break;
            }
            case ShaderHolder::ShaderInputElementType::uv:
            {
                input_descriptions.emplace_back(
                    "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, slot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0
                );
                break;
            }
            case ShaderHolder::ShaderInputElementType::count:
            {
                break;
            }
        }
    }

    return input_descriptions;
}

void ShaderHolder::create(HolderState& holder, const RenderDebug::DebugState& debug, const RenderContext::ContextState& context)
{
    for (uint8_t i = 0; i < (uint8_t)ShaderRenderType::count; ++i)
    {
        const ShaderRenderType shader_render = (ShaderRenderType)i;
        auto [vertex_shader_file, pixel_shader_file] = get_shader_file_names(shader_render);

        if (!std::filesystem::exists(vertex_shader_file))
        {
            Log::fatal(LOG_CAT, "Could not find {}.", vertex_shader_file);

            return;
        }

        if (!std::filesystem::exists(pixel_shader_file))
        {
            Log::fatal(LOG_CAT, "Could not find {}.", pixel_shader_file);

            return;
        }

        ShaderState& shader_state = holder.shaders.emplace_back();
        com_ptr<ID3DBlob> shader_error_message;

        HRESULT res = D3DCompileFromFile(
            fmt::detail::utf8_to_utf16(vertex_shader_file).c_str(), nullptr, nullptr, "main", "vs_5_0",
            D3D10_SHADER_OPTIMIZATION_LEVEL1 | D3DCOMPILE_DEBUG, 0, shader_state.vertex_bytecode.put(),
            shader_error_message.put()
        );
        RenderDebug::sh_check(debug, res, shader_error_message.get());

        if (!shader_state.vertex_bytecode)
        {
            Log::fatal(LOG_CAT, "Failed to load vertex shader code.");

            return;
        }

        com_ptr<ID3DBlob> pixel_shader_bytecode;
        res = D3DCompileFromFile(
            fmt::detail::utf8_to_utf16(pixel_shader_file).c_str(), nullptr, nullptr, "main", "ps_5_0",
            D3D10_SHADER_OPTIMIZATION_LEVEL1 | D3DCOMPILE_DEBUG, 0, pixel_shader_bytecode.put(),
            shader_error_message.put()
        );
       RenderDebug::sh_check(debug, res, shader_error_message.get());

        if (!pixel_shader_bytecode)
        {
            Log::fatal(LOG_CAT, "Failed to load pixel shader code.");

            return;
        }

        RenderDebug::check(debug,
            context.device->CreateVertexShader(
                shader_state.vertex_bytecode->GetBufferPointer(),
                shader_state.vertex_bytecode->GetBufferSize(), nullptr,
                shader_state.vertex.put()
            )
        );

        if (!shader_state.vertex)
        {
            Log::fatal(LOG_CAT, "Failed to create vertex shader.");

            return;
        }

        RenderDebug::check(debug,
            context.device->CreatePixelShader(
                pixel_shader_bytecode->GetBufferPointer(), pixel_shader_bytecode->GetBufferSize(),
                nullptr, shader_state.pixel.put()
            )
        );

        if (!shader_state.pixel)
        {
            Log::fatal(LOG_CAT, "Failed to create pixel shader.");

            return;
        }

        const std::vector<ShaderInputElementType> element_types = get_shader_input_element_types(shader_render);
        std::vector<D3D11_INPUT_ELEMENT_DESC> input_descriptions = get_shader_input_descriptions(element_types);

        RenderDebug::check(debug,
            context.device->CreateInputLayout(
                input_descriptions.data(),
                (uint32_t)std::size(input_descriptions),
                shader_state.vertex_bytecode->GetBufferPointer(),
                shader_state.vertex_bytecode->GetBufferSize(), shader_state.input_layout.put()
            )
        );
    }
}
