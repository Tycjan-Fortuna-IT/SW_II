#pragma once

#include "Core/Math/Vector3.hpp"

namespace SW {

    template <typename T>
        requires std::is_arithmetic_v<T>
    struct Matrix4 final
    {
        Matrix4(T x) {
            data[0] = static_cast<T>(x);
            data[5] = static_cast<T>(x);
            data[10] = static_cast<T>(x);
            data[15] = static_cast<T>(x);
        }

        Matrix4(const Matrix4& other) = default;
        Matrix4(Matrix4&& other) = default;
        Matrix4& operator=(const Matrix4& other) = default;
        Matrix4& operator=(Matrix4&& other) = default;

        ~Matrix4() = default;

        T data[16] = { static_cast<T>(0) };

        const std::string& ToString() const {
            static const std::string str =
                "\n{ " + std::to_string(data[0]) + ", " + std::to_string(data[1]) + ", " + std::to_string(data[2]) + ", " + std::to_string(data[3]) + "\n" +
                "  " + std::to_string(data[4]) + ", " + std::to_string(data[5]) + ", " + std::to_string(data[6]) + ", " + std::to_string(data[7]) + "\n" +
                "  " + std::to_string(data[8]) + ", " + std::to_string(data[9]) + ", " + std::to_string(data[10]) + ", " + std::to_string(data[11]) + "\n" +
                "  " + std::to_string(data[12]) + ", " + std::to_string(data[13]) + ", " + std::to_string(data[14]) + ", " + std::to_string(data[15]) + " }";

            return str;
        }

        const char* ToCString() const {
            return ToString().c_str();
        }

        const T* ValuePtr() const {
            return data;
        }

        void RotateX(f32 radians) {
            data[5] = static_cast<T>(cos(radians));
            data[6] = static_cast<T>(sin(radians));
            data[9] = static_cast<T>(-sin(radians));
            data[10] = static_cast<T>(cos(radians));
        }

        void RotateY(f32 radians) {
            data[0] = static_cast<T>(cos(radians));
            data[2] = static_cast<T>(-sin(radians));
            data[8] = static_cast<T>(sin(radians));
            data[10] = static_cast<T>(cos(radians));
        }

        void RotateZ(f32 radians) {
            data[0] = static_cast<T>(cos(radians));
            data[1] = static_cast<T>(sin(radians));
            data[4] = static_cast<T>(-sin(radians));
            data[5] = static_cast<T>(cos(radians));
        }
    };

}
