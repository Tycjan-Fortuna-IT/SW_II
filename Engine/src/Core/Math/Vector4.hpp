#pragma once

namespace SW {

    template <typename T>
    struct Vector4 final
    {
        Vector4(T x) {
            data[0] = static_cast<T>(x);
            data[1] = static_cast<T>(x);
            data[2] = static_cast<T>(x);
            data[3] = static_cast<T>(x);
        }

        Vector4(T x, T y, T z, T w) {
            data[0] = static_cast<T>(x);
            data[1] = static_cast<T>(y);
            data[2] = static_cast<T>(z);
            data[3] = static_cast<T>(w);
        }

        Vector4(const Vector4& other) = default;
        Vector4(Vector4&& other) = default;
        Vector4& operator=(const Vector4& other) = default;
        Vector4& operator=(Vector4&& other) = default;

        ~Vector4() = default;

        union {
            T data[4] = { static_cast<T>(0) };

            struct {
                union { T x, r; };
                union { T y, g; };
                union { T z, b; };
                union { T w, a; };

            };
        };

        std::string ToString() const {
            const std::string str = "{ " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + " }";

            return str;
        }

        const T* ValuePtr() const {
            return data;
        }
    };

}
