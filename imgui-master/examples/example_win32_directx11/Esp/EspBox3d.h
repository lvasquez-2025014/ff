#pragma once

// Esp_line.h ya trae todo el entorno de ESP:
//   vector.h (Vector3 global), adb.h (ReadZ, GetNodePosition), HD-Player.h (RenderWindow),
//   Core/Config.h, Core/Offsets.h, imgui_internal.h, y WorldToScreenImVec2
#include "Esp_line.h"

#include <cmath>
#include <algorithm>

namespace font3 {
    inline ImFont* fuentezada = nullptr;
}

inline ImColor box3dColorEnemy(0.11f, 0.55f, 0.98f, 1.0f);
inline ImColor box3dColorTeam(0.0f, 1.0f, 0.0f, 1.0f);

// Dibuja una arista entre dos puntos si ambos son validos en pantalla
inline void DrawBox3DEdge(ImDrawList* draw, const ImVec2& a, const ImVec2& b, ImU32 col, float thickness) {
    if (a.x < 1.0f || a.y < 1.0f || b.x < 1.0f || b.y < 1.0f) return;
    draw->AddLine(a, b, col, thickness);
}

// Rota un Vector3 global (Esp/vector.h) alrededor del eje Y
inline Vector3 RotateY(const Vector3& v, float angleRad) {
    float s = std::sin(angleRad);
    float c = std::cos(angleRad);
    return Vector3(
        v.x * c + v.z * s,
        v.y,
        -v.x * s + v.z * c
    );
}

void ESP_Box3D()
{
    auto& cfg = Core::GetConfig();

    // 1. Validar Ventana del Emulador
    if (!RenderWindow || !IsWindow(RenderWindow)) {
        const char* titles[] = { "BlueStacks App Player", "MSI App Player", "BlueStacks", "HD-Player", nullptr };
        for (int t = 0; titles[t]; t++) {
            HWND hParent = FindWindowA(NULL, titles[t]);
            if (hParent) {
                RenderWindow = FindHDPlayerWindow(hParent);
                if (!RenderWindow) RenderWindow = hParent;
                break;
            }
        }
        if (!RenderWindow || !IsWindow(RenderWindow)) return;
    }

    RECT clientRect;
    POINT screenPos = { 0, 0 };
    GetClientRect(RenderWindow, &clientRect);
    ClientToScreen(RenderWindow, &screenPos);

    int clientX = screenPos.x;
    int clientY = screenPos.y;
    int emuW = clientRect.right - clientRect.left;
    int emuH = clientRect.bottom - clientRect.top;
    if (emuW <= 0 || emuH <= 0) return;

    // 2. Validar Base Il2Cpp
    if (Il2Cpp == 0) return;

    uint32_t baseGameFacade = 0, gameFacade = 0, staticGameFacade = 0, currentGame = 0, currentMatch = 0, localPlayer = 0;
    if (!ReadZ(Il2Cpp + Core::Offsets::InitBase, baseGameFacade) || !baseGameFacade) return;
    if (!ReadZ(baseGameFacade, gameFacade) || !gameFacade) return;
    if (!ReadZ(gameFacade + Core::Offsets::StaticClass, staticGameFacade) || !staticGameFacade) return;
    if (!ReadZ(staticGameFacade, currentGame) || !currentGame) return;
    if (!ReadZ(currentGame + Core::Offsets::CurrentMatch, currentMatch) || !currentMatch) return;
    if (!ReadZ(currentMatch + Core::Offsets::LocalPlayer, localPlayer) || !localPlayer) return;

    // 3. ViewMatrix (mismo tipo que Esp_line.h)
    Matrix4x4 viewMatrix = {};
    uint32_t followCamera = 0, camera = 0, cameraBase = 0;
    if (!ReadZ(localPlayer + Core::Offsets::FollowCamera, followCamera) || !followCamera ||
        !ReadZ(followCamera + Core::Offsets::Camera, camera) || !camera ||
        !ReadZ(camera + 0x8, cameraBase) || !cameraBase ||
        !ReadZ(cameraBase + Core::Offsets::ViewMatrix, viewMatrix)) return;

    // Lectura opcional de rotacion del jugador (yaw) para orientar la caja
    uint32_t aimRotPtr = 0;
    float aimYaw = 0.0f;
    if (ReadZ(localPlayer + Core::Offsets::AimRotation, aimRotPtr) && aimRotPtr) {
        Vector3 rot = {};
        if (ReadZ(aimRotPtr, rot)) {
            aimYaw = rot.y;
        }
    }

    // 4. Entidades
    uint32_t entityDictionary = 0;
    if (!ReadZ(currentGame + Core::Offsets::DictionaryEntities, entityDictionary) || !entityDictionary) return;

    uint32_t entities = 0; ReadZ(entityDictionary + 0x0C, entities); entities += 0x1C;
    int entitiesCount = 0; ReadZ(entityDictionary + 0x10, entitiesCount);
    if (entitiesCount <= 0 || entitiesCount > 150) return;

    // 5. Preparar ImGui
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    drawList->PushClipRect(ImVec2(clientX, clientY), ImVec2(clientX + emuW, clientY + emuH), true);

    // Colores y grosor
    ImU32 colBoxEnemy = ImGui::ColorConvertFloat4ToU32(cfg.ColorBox);
    ImU32 colBoxTeam  = ImGui::ColorConvertFloat4ToU32(cfg.ColorTeam);
    float thickness   = cfg.BoxThickness;

    // 6. Iterar enemigos
    for (int i = 0; i < entitiesCount; i++) {
        uint32_t entity = 0;
        if (!ReadZ(entities + (i * 0x10), entity) || entity == 0 || entity == localPlayer) continue;

        bool isDead = false;
        if (ReadZ(entity + Core::Offsets::Player_IsDead, isDead) && isDead) continue;

        bool isTeam = false;
        uint32_t avatarManager = 0, avatar = 0, umaData = 0;
        if (ReadZ(entity + Core::Offsets::AvatarManager, avatarManager) && avatarManager) {
            if (ReadZ(avatarManager + Core::Offsets::Avatar, avatar) && avatar) {
                if (ReadZ(avatar + Core::Offsets::Avatar_Data, umaData) && umaData) {
                    ReadZ(umaData + Core::Offsets::Avatar_Data_IsTeam, isTeam);
                }
            }
        }
        if (isTeam && !cfg.ShowTeam) continue;

        // Obtener posiciones de cabeza y raiz
        uint32_t headNode = 0, rootNode = 0;
        Vector3 headPos = { 0.0f, 0.0f, 0.0f };
        Vector3 rootPos = { 0.0f, 0.0f, 0.0f };

        if (!ReadZ(entity + Core::Offsets::Head, headNode) || headNode == 0) continue;
        if (!GetNodePosition(headNode, headPos)) continue;
        if (headPos.x == 0.0f && headPos.y == 0.0f && headPos.z == 0.0f) continue;

        if (ReadZ(entity + Core::Offsets::Root, rootNode) && rootNode != 0) {
            GetNodePosition(rootNode, rootPos);
        }

        // Calcular centro, altura y ancho de la caja
        float groundY = (rootPos.y != 0.0f) ? rootPos.y : (headPos.y - 1.65f);
        float playerHeight = headPos.y - groundY;
        if (playerHeight < 0.6f) playerHeight = 0.6f;
        if (playerHeight > 2.3f) playerHeight = 1.87f;

        Vector3 centerWorld(headPos.x, groundY + playerHeight * 0.5f, headPos.z);

        // Profundidad para descartar detras de la camara
        float cameraDepth = (centerWorld.x * viewMatrix.m03) + (centerWorld.y * viewMatrix.m13) + (centerWorld.z * viewMatrix.m23) + viewMatrix.m33;
        if (cameraDepth < 0.5f) continue;

        // Tamano de la caja (proporcion a la altura)
        float boxW = playerHeight * 0.55f;
        float boxD = playerHeight * 0.45f;
        float boxH = playerHeight;

        ImU32 colBox = isTeam ? colBoxTeam : colBoxEnemy;

        // Generar 8 vertices del cubo en espacio mundial
        // 0 = abajo-izq-atras, 1 = abajo-der-atras, 2 = abajo-der-frente, 3 = abajo-izq-frente
        // 4 = arriba-izq-atras, 5 = arriba-der-atras, 6 = arriba-der-frente, 7 = arriba-izq-frente
        Vector3 localVerts[8] = {
            Vector3(-boxW * 0.5f, -boxH * 0.5f, -boxD * 0.5f),
            Vector3( boxW * 0.5f, -boxH * 0.5f, -boxD * 0.5f),
            Vector3( boxW * 0.5f, -boxH * 0.5f,  boxD * 0.5f),
            Vector3(-boxW * 0.5f, -boxH * 0.5f,  boxD * 0.5f),
            Vector3(-boxW * 0.5f,  boxH * 0.5f, -boxD * 0.5f),
            Vector3( boxW * 0.5f,  boxH * 0.5f, -boxD * 0.5f),
            Vector3( boxW * 0.5f,  boxH * 0.5f,  boxD * 0.5f),
            Vector3(-boxW * 0.5f,  boxH * 0.5f,  boxD * 0.5f),
        };

        // Aplicar rotacion en Y (orientar segun aim del jugador)
        Vector3 worldVerts[8];
        for (int v = 0; v < 8; v++) {
            Vector3 rotated = RotateY(localVerts[v], aimYaw);
            worldVerts[v] = centerWorld + rotated;
        }

        // Proyectar los 8 vertices a pantalla (misma firma que Esp_line.h)
        ImVec2 screenVerts[8];
        bool allValid = true;
        for (int v = 0; v < 8; v++) {
            ImVec2 s = WorldToScreenImVec2(viewMatrix, worldVerts[v], emuW, emuH);
            if (s.x < 1.0f || s.y < 1.0f) {
                allValid = false;
                break;
            }
            screenVerts[v] = ImVec2(clientX + s.x, clientY + s.y);
        }

        if (!allValid) continue;

        // Dibujar las 12 aristas del cubo
        // Cara inferior: 0-1-2-3-0
        DrawBox3DEdge(drawList, screenVerts[0], screenVerts[1], colBox, thickness);
        DrawBox3DEdge(drawList, screenVerts[1], screenVerts[2], colBox, thickness);
        DrawBox3DEdge(drawList, screenVerts[2], screenVerts[3], colBox, thickness);
        DrawBox3DEdge(drawList, screenVerts[3], screenVerts[0], colBox, thickness);

        // Cara superior: 4-5-6-7-4
        DrawBox3DEdge(drawList, screenVerts[4], screenVerts[5], colBox, thickness);
        DrawBox3DEdge(drawList, screenVerts[5], screenVerts[6], colBox, thickness);
        DrawBox3DEdge(drawList, screenVerts[6], screenVerts[7], colBox, thickness);
        DrawBox3DEdge(drawList, screenVerts[7], screenVerts[4], colBox, thickness);

        // Aristas verticales: 0-4, 1-5, 2-6, 3-7
        DrawBox3DEdge(drawList, screenVerts[0], screenVerts[4], colBox, thickness);
        DrawBox3DEdge(drawList, screenVerts[1], screenVerts[5], colBox, thickness);
        DrawBox3DEdge(drawList, screenVerts[2], screenVerts[6], colBox, thickness);
        DrawBox3DEdge(drawList, screenVerts[3], screenVerts[7], colBox, thickness);
    }

    drawList->PopClipRect();
}
