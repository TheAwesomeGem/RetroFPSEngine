//
// Created by TheAwesomeGem on 11/27/2025.
//

#pragma once

#include <array>
#include <d3d11.h>
#include <SimpleMath.h>
#include <wil/com.h>

#define NOT_COPYABLE(TypeName) \
TypeName (TypeName const&) = delete; \
TypeName & operator = ( TypeName const& ) = delete;

#define NOT_COPYABLE_DEFAULT_MOVEABLE(TypeName) \
TypeName (TypeName const&) = delete; \
TypeName & operator = ( TypeName const& ) = delete; \
TypeName (TypeName &&) = default; \
TypeName& operator = (TypeName&&) = default;

#define NOT_MOVEABLE(TypeName) \
TypeName (TypeName &&) = delete; \
TypeName& operator = (TypeName&&) = delete;

#define NOT_COPYABLE_AND_MOVEABLE(TypeName) \
TypeName ( TypeName const& ) = delete; \
TypeName & operator = ( TypeName const& ) = delete; \
TypeName ( TypeName && ) = delete; \
TypeName & operator = ( TypeName && ) = delete;

using Vec2F = DirectX::SimpleMath::Vector2;
using Vec3F = DirectX::SimpleMath::Vector3;
using Vec4F = DirectX::SimpleMath::Vector4;
using Mat4 = DirectX::SimpleMath::Matrix;
using Quaternion = DirectX::SimpleMath::Quaternion;
using wil::com_ptr;
using Color = DirectX::SimpleMath::Color;

struct Vec2I
{
    int x = 0;
    int y = 0;
};

namespace Common
{
    inline constexpr size_t INVALID_SCENE_IDX = std::numeric_limits<size_t>::max();
    inline constexpr size_t INVALID_ACTOR_IDX = std::numeric_limits<size_t>::max();
    inline constexpr std::array<uint32_t, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT> NO_OFFSETS = {};
}
