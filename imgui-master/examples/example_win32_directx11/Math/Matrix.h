#pragma once
#include <cstring>
#include "Vector.h"

namespace Math {

    struct Matrix4x4 {
        // Usamos uniones para poder acceder por nombre (m00) o por arreglo (m[0])
        union {
            struct {
                float m00, m01, m02, m03;
                float m10, m11, m12, m13;
                float m20, m21, m22, m23;
                float m30, m31, m32, m33;
            };
            float m[16];
        };

        Matrix4x4() {
            //  MEJORA: Inicialización rápida a ceros 
            std::memset(this, 0, sizeof(Matrix4x4));
        }

        static inline Matrix4x4 Identity() {
            Matrix4x4 mat;
            mat.m00 = 1.0f; mat.m11 = 1.0f; mat.m22 = 1.0f; mat.m33 = 1.0f;
            return mat;
        }
    };

    static inline Matrix4x4 QuaternionToMatrix(const Quaternion& q) {
        Matrix4x4 m = Matrix4x4::Identity();
        float xx = q.x * q.x, yy = q.y * q.y, zz = q.z * q.z;
        float xy = q.x * q.y, xz = q.x * q.z, yz = q.y * q.z;
        float wx = q.w * q.x, wy = q.w * q.y, wz = q.w * q.z;

        m.m00 = 1.0f - 2.0f * (yy + zz);
        m.m01 = 2.0f * (xy - wz);
        m.m02 = 2.0f * (xz + wy);

        m.m10 = 2.0f * (xy + wz);
        m.m11 = 1.0f - 2.0f * (xx + zz);
        m.m12 = 2.0f * (yz - wx);

        m.m20 = 2.0f * (xz - wy);
        m.m21 = 2.0f * (yz + wx);
        m.m22 = 1.0f - 2.0f * (xx + yy);

        return m;
    }

} // namespace Math
