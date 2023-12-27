#pragma once

namespace SW {

    namespace Math {

        constexpr f32 ToRadians(f32 degrees) {
            return degrees * 0.017453292519943295f;
        }

        constexpr f32 ToDegrees(f32 radians) {
            return radians * 57.29577951308232f;
        }

        constexpr f32 PI = 3.14159265358979323846264338327950288f;

    }

}
