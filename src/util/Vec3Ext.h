//
// Created by TheAwesomeGem on 11/30/2025.
//

#pragma once
#include "src/Common.h"

namespace Vec3Ext
{
    inline Vec3F xz_plane(Vec3F vector)
    {
        return { vector.x, 0.0F, vector.z };
    }
}
