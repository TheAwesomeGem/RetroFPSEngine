//
// Created by TheAwesomeGem on 12/9/2025.
//

#pragma once
#include <DirectXMath.h>

namespace MathExt
{
    inline float rot_mod(float angle, float amount)
    {
        return DirectX::XMScalarModAngle(angle + DirectX::XMConvertToRadians(amount));
    }
}
