#pragma once

namespace SW {

    template <typename T>
    struct Vector3 final
    {
        Vector3(T x) {
            data[0] = static_cast<T>(x);
            data[1] = static_cast<T>(x);
            data[2] = static_cast<T>(x);
        }

        Vector3(T x, T y, T z) {
            data[0] = static_cast<T>(x);
            data[1] = static_cast<T>(y);
            data[2] = static_cast<T>(z);
        }

        Vector3(const Vector3& other) = default;
        Vector3(Vector3&& other) = default;
        Vector3& operator=(const Vector3& other) = default;
        Vector3& operator=(Vector3&& other) = default;

        ~Vector3() = default;

        union {
            T data[3] = { static_cast<T>(0) };

            struct {
                union { T x, r, s, u; };
                union { T y, g, t, v; };
                union { T z, b, p, w; };
            };
        };

        std::string ToString() const {
            const std::string str = "{ " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + " }";

            return str;
        }

        const T* ValuePtr() const {
            return data;
        }

        T Length() const {
            return static_cast<T>(sqrt(x * x + y * y + z * z));
        }

        void Normalize() {
            const T length = Length();

            x /= length;
            y /= length;
            z /= length;
        }
    };

}
