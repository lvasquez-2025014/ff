#pragma once
#include "Vector.h"
#include "Matrix.h"
#include <imgui.h>

namespace Math {

    // Retorna true si el punto está visible en pantalla, y guarda el resultado en outScreen
    inline bool WorldToScreen(const Matrix4x4& viewMatrix, const Vector3& pos, int width, int height, Vector2& outScreen) {
        // Multiplicación manual (Rápida y sin dependencias)
        float clipX = (pos.x * viewMatrix.m00) + (pos.y * viewMatrix.m10) + (pos.z * viewMatrix.m20) + viewMatrix.m30;
        float clipY = (pos.x * viewMatrix.m01) + (pos.y * viewMatrix.m11) + (pos.z * viewMatrix.m21) + viewMatrix.m31;
        float clipW = (pos.x * viewMatrix.m03) + (pos.y * viewMatrix.m13) + (pos.z * viewMatrix.m23) + viewMatrix.m33;

        // Si clipW es menor a 0.1f, el punto está detrás de la cámara
        if (clipW < 0.1f) {
            outScreen = Vector2(-1.0f, -1.0f);
            return false;
        }

        // Normalizar coordenadas
        float invW = 1.0f / clipW;
        float ndcX = clipX * invW;
        float ndcY = clipY * invW;

        // Convertir a coordenadas de pantalla (NDC a Pixels)
        float halfW = static_cast<float>(width) * 0.5f;
        float halfH = static_cast<float>(height) * 0.5f;

        outScreen.x = halfW + (halfW * ndcX);
        outScreen.y = halfH - (halfH * ndcY); // Invertir Y para ImGui

        return true;
    }

    // Wrapper directo para ImGui
    inline ImVec2 WorldToScreenImVec2(const Matrix4x4& viewMatrix, const Vector3& pos, int width, int height) {
        Vector2 res;
        if (WorldToScreen(viewMatrix, pos, width, height, res)) {
            return ImVec2(res.x, res.y);
        }
        return ImVec2(-1.0f, -1.0f); // Falla segura
    }

} // namespace Math
