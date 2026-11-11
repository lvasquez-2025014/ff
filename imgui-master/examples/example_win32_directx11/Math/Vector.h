#pragma once
#include <cmath>
#include <algorithm>

namespace Math {

    struct Vector2 {
        float x = 0.0f;
        float y = 0.0f;

        Vector2() : x(0.0f), y(0.0f) {}
        Vector2(float _x, float _y) : x(_x), y(_y) {}

        // Operadores básicos
        inline Vector2 operator+(const Vector2& o) const { return Vector2(x + o.x, y + o.y); }
        inline Vector2 operator-(const Vector2& o) const { return Vector2(x - o.x, y - o.y); }
        inline Vector2 operator*(float s) const { return Vector2(x * s, y * s); }
        inline Vector2 operator/(float s) const { return Vector2(x / s, y / s); }

        // Operadores compuestos (más rápidos de escribir)
        inline Vector2& operator+=(const Vector2& o) { x += o.x; y += o.y; return *this; }
        inline Vector2& operator-=(const Vector2& o) { x -= o.x; y -= o.y; return *this; }

        inline float Length() const { return std::sqrt(x * x + y * y); }
        inline float SqrLength() const { return x * x + y * y; } // 👈 MEJORA: Ideal para comparar distancias sin sqrt
        inline float Distance(const Vector2& o) const { return (*this - o).Length(); }
        inline float Dot(const Vector2& o) const { return x * o.x + y * o.y; }

        static inline Vector2 Zero() { return Vector2(0.0f, 0.0f); }
    };

    struct Vector3 {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
        Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

        inline Vector3 operator+(const Vector3& o) const { return Vector3(x + o.x, y + o.y, z + o.z); }
        inline Vector3 operator-(const Vector3& o) const { return Vector3(x - o.x, y - o.y, z - o.z); }
        inline Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }
        inline Vector3 operator/(float s) const { return Vector3(x / s, y / s, z / s); }

        inline Vector3& operator+=(const Vector3& o) { x += o.x; y += o.y; z += o.z; return *this; }
        inline Vector3& operator-=(const Vector3& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }

        inline bool operator==(const Vector3& o) const { return x == o.x && y == o.y && z == o.z; }
        inline bool operator!=(const Vector3& o) const { return !(*this == o); }

        inline float Length() const { return std::sqrt(x * x + y * y + z * z); }
        inline float SqrLength() const { return x * x + y * y + z * z; } // 👈 MEJORA
        inline float Distance(const Vector3& o) const { return (*this - o).Length(); }
        inline float Dot(const Vector3& o) const { return x * o.x + y * o.y + z * o.z; }

        // 👈 MEJORA: Producto Cruz (Esencial para matemáticas 3D)
        inline Vector3 Cross(const Vector3& o) const {
            return Vector3(
                y * o.z - z * o.y,
                z * o.x - x * o.z,
                x * o.y - y * o.x
            );
        }

        inline Vector3 Normalized() const {
            float len = Length();
            if (len > 0.0001f) {
                return *this / len;
            }
            return Vector3(0.0f, 0.0f, 0.0f);
        }

        static inline Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
    };

    struct Vector4 {
        float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
        Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
        Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
    };

    struct Quaternion {
        float x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;
        Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
        Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
    };

} // namespace Math
