//
// Created by TheAwesomeGem on 11/29/2025.
//

#pragma once
#include <SimpleMath.h>

namespace Mat4Ext
{
    inline DirectX::SimpleMath::Matrix CreatePerspectiveFieldOfViewLH(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept // NOLINT(*-identifier-naming)
    {
        using namespace DirectX;
        SimpleMath::Matrix R; // NOLINT(*-identifier-naming)
        XMStoreFloat4x4(&R, XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));
        return R;
    }

    inline DirectX::SimpleMath::Matrix CreateLookAtLH(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target, const DirectX::SimpleMath::Vector3& up) noexcept
    {
        using namespace DirectX;
        SimpleMath::Matrix R;
        const XMVECTOR eyev = XMLoadFloat3(&eye);
        const XMVECTOR targetv = XMLoadFloat3(&target);
        const XMVECTOR upv = XMLoadFloat3(&up);
        XMStoreFloat4x4(&R, XMMatrixLookAtLH(eyev, targetv, upv));
        return R;
    }

    inline DirectX::SimpleMath::Matrix StripTranslation(DirectX::SimpleMath::Matrix mat)
    {
        mat._41 = 0.0F;
        mat._42 = 0.0F;
        mat._43 = 0.0F;

        return mat;
    }
}
