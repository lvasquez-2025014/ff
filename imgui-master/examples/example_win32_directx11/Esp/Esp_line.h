#pragma once

#include "vector.h"
#include "adb.h"
#include "../Core/Config.h"
#include "../Core/Offsets.h"

#include <imgui_internal.h>
#include <chrono>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

double getCurrentTime() {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now).count() / 1000.0;
}

namespace font {
    inline ImFont* fuentezada = nullptr;
    inline ImFont* onod = nullptr;
}
namespace font2 { extern ImFont* fuentezada; }

inline ImFont* medium_font[30];
char defaultName[255] = "";
inline ImColor lineaverdexdddd(0, 255, 0);

// Variables de configuración global (Tus originales)
ImVec4 hongoColor(0.0f, 0.0f, 1.0f, 1.0f);
ImVec4 fillBoxColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
ImVec4 namecolor(1.0f, 0.823f, 0.0f, 1.0f);
ImVec4 lineColorEnemy(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 lineColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImU32 invisibleColor = IM_COL32(160, 32, 240, 200);
ImVec4 BoxColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 healthColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 WeaponsColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 Weaponstext = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 bonesColor = ImVec4(255, 182, 193, 255);
ImVec4 bonesColorCIRCULOS = ImVec4(0, 128, 255, 255);
ImVec4 fovColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 crosshairColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
ImVec4 userColorAmarillo = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
inline ImVec4 ColorFov = ImColor(255, 255, 255);
ImVec4 boxColor(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 lineColorTeam(0.0f, 1.0f, 0.0f, 1.0f);

const char* cheat_name = " ";
std::string EspADB = "/ostorga";
int g_windowWidth = GetSystemMetrics(SM_CXSCREEN);
int g_windowHeight = GetSystemMetrics(SM_CYSCREEN);
HWND hdPlayerWindow = nullptr;
static auto clickStartTime = std::chrono::steady_clock::time_point();
bool PullPlayer = false;
extern bool Main_Window;

inline namespace Main {
    bool EspMode = false;
    bool EnabledEsp = false;
}

int RadioMagnet = 200.0f;
std::chrono::steady_clock::time_point lastFireTime;
const int fireCooldownMs360 = 100;
float PullDis = 300.0f;
float PullFov = 300.0f;
float pullTime = 0.0f;
float suavidad360 = 2.0f;

int VidaType = 0;
const char* items5[] = { "Right", "Left", "Below" };

int TpSelection = 0;
const char* listTp[] = { "Neck", "Head", "Chest" };

bool ESPFillBox = false;
bool ESPHongo = false;

struct PlayerData {
    uint32_t entity;
    std::string name;
    float distance;
};
std::vector<PlayerData> detectedPlayers;

// Variables booleanas globales que tu Feature sincroniza
bool EspEnabled = false;
bool EspBox = false, ESPBox = false, ESPBones = false, EspLineZ = true, ESPNameZ = false;
bool ShowFov = false, Shadow = false, rainbowMode = false, rgbMode = false;
static bool ShowTeam = false, ShowInvis = true, ShowWukongOrion = true;
bool ESPHealth = false;

float lineThicknessBase = 1.5f;
int maxDistance = 500;
int FovAll = 150;
int DistanceAll = 500;

// ═════════════════════════════════════════════════════════════════════
// FUNCIONES DE DIBUJADO VISUAL
// ═════════════════════════════════════════════════════════════════════

// Línea con punta de flecha y efecto glow
inline void DrawModernLine(ImDrawList* drawList, ImVec2 start, ImVec2 end, ImU32 col, float thickness) {
    drawList->AddLine(start, end, IM_COL32(0, 0, 0, 120), thickness + 2.0f);
    drawList->AddLine(start, end, col, thickness);
    ImVec2 dir = ImVec2(end.x - start.x, end.y - start.y);
    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (len > 0.0f) {
        dir = ImVec2(dir.x / len, dir.y / len);
        ImVec2 perp = ImVec2(-dir.y, dir.x);
        float arrowSize = 6.0f;
        ImVec2 arrowBack = ImVec2(end.x - dir.x * arrowSize, end.y - dir.y * arrowSize);
        ImVec2 arrowLeft = ImVec2(arrowBack.x + perp.x * 4.0f, arrowBack.y + perp.y * 4.0f);
        ImVec2 arrowRight = ImVec2(arrowBack.x - perp.x * 4.0f, arrowBack.y - perp.y * 4.0f);
        drawList->AddLine(arrowBack, arrowLeft, col, thickness);
        drawList->AddLine(arrowBack, arrowRight, col, thickness);
    }
}

// Corner Box moderno con borde interior y exterior
inline void DrawModernBox(ImDrawList* drawList, float x, float y, float w, float h, ImU32 col, float thickness) {
    float shadowThick = thickness + 1.5f;
    drawList->AddRect(ImVec2(x - 1, y - 1), ImVec2(x + w + 1, y + h + 1), IM_COL32(0, 0, 0, 180), 0.0f, 0, shadowThick);

    float cornerW = w * 0.28f;
    float cornerH = h * 0.28f;

    drawList->AddLine(ImVec2(x, y), ImVec2(x + cornerW, y), col, thickness);
    drawList->AddLine(ImVec2(x, y), ImVec2(x, y + cornerH), col, thickness);
    drawList->AddLine(ImVec2(x + w, y), ImVec2(x + w - cornerW, y), col, thickness);
    drawList->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + cornerH), col, thickness);
    drawList->AddLine(ImVec2(x, y + h), ImVec2(x + cornerW, y + h), col, thickness);
    drawList->AddLine(ImVec2(x, y + h), ImVec2(x, y + h - cornerH), col, thickness);
    drawList->AddLine(ImVec2(x + w, y + h), ImVec2(x + w - cornerW, y + h), col, thickness);
    drawList->AddLine(ImVec2(x + w, y + h), ImVec2(x + w, y + h - cornerH), col, thickness);

    ImU32 innerCol = IM_COL32(255, 255, 255, 60);
    float innerX = x + 3.0f, innerY = y + 3.0f;
    float innerW = w - 6.0f, innerH = h - 6.0f;
    float icornerW = innerW * 0.20f;
    float icornerH = innerH * 0.20f;

    drawList->AddLine(ImVec2(innerX, innerY), ImVec2(innerX + icornerW, innerY), innerCol, 1.0f);
    drawList->AddLine(ImVec2(innerX, innerY), ImVec2(innerX, innerY + icornerH), innerCol, 1.0f);
    drawList->AddLine(ImVec2(innerX + innerW, innerY), ImVec2(innerX + innerW - icornerW, innerY), innerCol, 1.0f);
    drawList->AddLine(ImVec2(innerX + innerW, innerY), ImVec2(innerX + innerW, innerY + icornerH), innerCol, 1.0f);
    drawList->AddLine(ImVec2(innerX, innerY + innerH), ImVec2(innerX + icornerW, innerY + innerH), innerCol, 1.0f);
    drawList->AddLine(ImVec2(innerX, innerY + innerH), ImVec2(innerX, innerY + innerH - icornerH), innerCol, 1.0f);
    drawList->AddLine(ImVec2(innerX + innerW, innerY + innerH), ImVec2(innerX + innerW - icornerW, innerY + innerH), innerCol, 1.0f);
    drawList->AddLine(ImVec2(innerX + innerW, innerY + innerH), ImVec2(innerX + innerW, innerY + innerH - icornerH), innerCol, 1.0f);
}

// Barra de salud moderna con borde y porcentaje
inline void DrawModernHealthBar(ImDrawList* drawList, float boxX, float boxY, float boxH, int health) {
    if (health <= 0) return;

    float barW = 4.0f;
    float barX = boxX - 7.0f;
    float healthPct = std::clamp((float)health / 200.0f, 0.0f, 1.0f);

    drawList->AddRectFilled(ImVec2(barX - 1.0f, boxY - 1.0f), ImVec2(barX + barW + 1.0f, boxY + boxH + 1.0f), IM_COL32(10, 12, 18, 220), 2.0f);
    drawList->AddRect(ImVec2(barX - 1.0f, boxY - 1.0f), ImVec2(barX + barW + 1.0f, boxY + boxH + 1.0f), IM_COL32(40, 45, 55, 200), 2.0f, 0, 1.0f);

    ImU32 hpCol;
    if (healthPct > 0.65f) hpCol = IM_COL32(60, 220, 120, 255);
    else if (healthPct > 0.35f) hpCol = IM_COL32(255, 190, 40, 255);
    else hpCol = IM_COL32(255, 60, 80, 255);

    drawList->AddRectFilled(ImVec2(barX - 1.0f, boxY - 1.0f), ImVec2(barX + barW + 1.0f, boxY + boxH + 1.0f), IM_COL32(hpCol >> 24 & 0xFF, hpCol >> 16 & 0xFF, hpCol >> 8 & 0xFF, 30), 2.0f);

    float fillH = boxH * healthPct;
    float fillY = boxY + boxH - fillH;
    drawList->AddRectFilled(ImVec2(barX, fillY), ImVec2(barX + barW, boxY + boxH), hpCol, 1.5f);
    drawList->AddLine(ImVec2(barX, fillY), ImVec2(barX + barW, fillY), IM_COL32(255, 255, 255, 100), 1.0f);
}

// Badge de nombre moderno con fondo translúcido y borde de acento
inline void DrawModernNameBadge(ImDrawList* drawList, float headX, float topY, const std::string& name, ImU32 col, float boxH) {
    if (name.empty()) return;

    ImFont* font = font2::fuentezada ? font2::fuentezada : ImGui::GetFont();
    float fontSize = (boxH < 85.0f) ? std::max(8.0f, 10.5f * (boxH / 85.0f)) : 10.5f;

    ImVec2 txtSz = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, name.c_str());
    float padX = 6.0f;
    float padY = 2.5f;

    float badgeX = headX - (txtSz.x * 0.5f) - padX;
    float badgeY = topY - txtSz.y - padY - 6.0f;

    drawList->AddRectFilled(ImVec2(badgeX + 1.0f, badgeY + 1.0f), ImVec2(badgeX + txtSz.x + padX * 2 + 1.0f, badgeY + txtSz.y + padY * 2 + 1.0f), IM_COL32(0, 0, 0, 150), 4.0f);
    drawList->AddRectFilled(ImVec2(badgeX, badgeY), ImVec2(badgeX + txtSz.x + padX * 2, badgeY + txtSz.y + padY * 2), IM_COL32(15, 17, 23, 230), 4.0f);
    drawList->AddRect(ImVec2(badgeX, badgeY), ImVec2(badgeX + txtSz.x + padX * 2, badgeY + txtSz.y + padY * 2), IM_COL32(col >> 24 & 0xFF, col >> 16 & 0xFF, col >> 8 & 0xFF, 120), 4.0f, 0, 1.0f);
    drawList->AddLine(ImVec2(badgeX, badgeY), ImVec2(badgeX + txtSz.x + padX * 2, badgeY), IM_COL32(col >> 24 & 0xFF, col >> 16 & 0xFF, col >> 8 & 0xFF, 255), 1.5f);
    drawList->AddText(font, fontSize, ImVec2(badgeX + padX + 1.0f, badgeY + padY + 1.0f), IM_COL32(0, 0, 0, 180), name.c_str());
    drawList->AddText(font, fontSize, ImVec2(badgeX + padX, badgeY + padY), col, name.c_str());
}

// Badge de distancia moderno
inline void DrawModernDistanceBadge(ImDrawList* drawList, float headX, float topY, float distance, ImU32 col, float boxH) {
    if (distance <= 0.0f) return;

    ImFont* font = font2::fuentezada ? font2::fuentezada : ImGui::GetFont();
    float fontSize = 9.0f;

    char distBuf[32];
    sprintf_s(distBuf, "%.1fm", distance);
    std::string distStr = distBuf;

    ImVec2 txtSz = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, distStr.c_str());
    float padX = 4.0f;
    float padY = 1.5f;

    float badgeX = headX - (txtSz.x * 0.5f) - padX;
    float badgeY = topY - txtSz.y - padY - 4.0f;

    drawList->AddRectFilled(ImVec2(badgeX, badgeY), ImVec2(badgeX + txtSz.x + padX * 2, badgeY + txtSz.y + padY * 2), IM_COL32(10, 12, 18, 200), 3.0f);
    drawList->AddRect(ImVec2(badgeX, badgeY), ImVec2(badgeX + txtSz.x + padX * 2, badgeY + txtSz.y + padY * 2), IM_COL32(col >> 24 & 0xFF, col >> 16 & 0xFF, col >> 8 & 0xFF, 100), 3.0f, 0, 1.0f);
    drawList->AddText(font, fontSize, ImVec2(badgeX + padX, badgeY + padY), IM_COL32(255, 255, 255, 230), distStr.c_str());
}

// Leer cadena wide (UTF-16) desde memoria del juego
inline std::wstring ReadWideString(uintptr_t nameAddr, size_t maxLength = 32) {
    std::wstring result;
    for (size_t i = 0; i < maxLength; i++) {
        uint16_t wchar_value = 0;
        if (!ReadZ(nameAddr + 0xC + (i * 2), wchar_value)) break;
        if (wchar_value == 0x0000) break;
        result += static_cast<wchar_t>(wchar_value);
    }
    return result;
}

// Convertir UTF-16 a UTF-8
inline std::string ConvertUTF16ToUTF8(const std::wstring& wstr) {
    if (wstr.empty()) return "";
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (size_needed <= 0) return "";
    std::string utf8Str(size_needed - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8Str[0], size_needed, nullptr, nullptr);
    return utf8Str;
}

inline std::string ReadPlayerName(uintptr_t nameAddr) {
    return ConvertUTF16ToUTF8(ReadWideString(nameAddr));
}

void ESP_line()
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

    // 3. ViewMatrix
    Matrix4x4 viewMatrix = {};
    uint32_t followCamera = 0, camera = 0, cameraBase = 0;
    if (!ReadZ(localPlayer + Core::Offsets::FollowCamera, followCamera) || !followCamera ||
        !ReadZ(followCamera + Core::Offsets::Camera, camera) || !camera ||
        !ReadZ(camera + 0x8, cameraBase) || !cameraBase ||
        !ReadZ(cameraBase + Core::Offsets::ViewMatrix, viewMatrix)) return;

    // 4. Entidades
    uint32_t entityDictionary = 0;
    if (!ReadZ(currentGame + Core::Offsets::DictionaryEntities, entityDictionary) || !entityDictionary) return;

    uint32_t entities = 0; ReadZ(entityDictionary + 0x0C, entities); entities += 0x1C;
    int entitiesCount = 0; ReadZ(entityDictionary + 0x10, entitiesCount);
    if (entitiesCount <= 0 || entitiesCount > 150) return;

    // 5. Preparar ImGui
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    
    // 🔥 CORRECCIÓN: Recorte de pantalla (Evita dibujar fuera de BlueStacks)
    drawList->PushClipRect(ImVec2(clientX, clientY), ImVec2(clientX + emuW, clientY + emuH), true);

    // --- 5.1. DIBUJAR CÍRCULO FOV (Si está activado) ---
    if (cfg.SilentAimEnabled && cfg.DrawSilentFov) {
        ImVec2 center(clientX + (emuW * 0.5f), clientY + (emuH * 0.5f));
        ImU32 colFov = ImGui::ColorConvertFloat4ToU32(cfg.ColorSilentFov);
        
        // Halo exterior
        drawList->AddCircle(center, cfg.SilentFov + 1.5f, IM_COL32(29, 140, 248, 35), 64, 2.5f);
        // Círculo principal
        drawList->AddCircle(center, cfg.SilentFov, colFov, 64, 1.2f);
        
        // Ticks
        float tick = 6.0f;
        drawList->AddLine(ImVec2(center.x, center.y - cfg.SilentFov - tick), ImVec2(center.x, center.y - cfg.SilentFov + 2), colFov, 1.5f);
        drawList->AddLine(ImVec2(center.x, center.y + cfg.SilentFov - 2), ImVec2(center.x, center.y + cfg.SilentFov + tick), colFov, 1.5f);
        drawList->AddLine(ImVec2(center.x - cfg.SilentFov - tick, center.y), ImVec2(center.x - cfg.SilentFov + 2, center.y), colFov, 1.5f);
        drawList->AddLine(ImVec2(center.x + cfg.SilentFov - 2, center.y), ImVec2(center.x + cfg.SilentFov + tick, center.y), colFov, 1.5f);
        
        drawList->AddCircleFilled(center, 2.0f, colFov);
    }

    ImVec2 lineStart(clientX + (emuW * 0.5f), clientY + 5.0f);

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

        uint32_t headNode = 0;
        Vector3 headPos = { 0.0f, 0.0f, 0.0f };
        if (!ReadZ(entity + Core::Offsets::Head, headNode) || headNode == 0) continue;
        if (!GetNodePosition(headNode, headPos)) continue;
        if (headPos.x == 0.0f && headPos.y == 0.0f && headPos.z == 0.0f) continue;

        ImVec2 headScreen = WorldToScreenImVec2(viewMatrix, headPos, emuW, emuH);
        if (headScreen.x < 1.0f || headScreen.y < 1.0f) continue;
        if (headScreen.x <= 0 || headScreen.x >= emuW || headScreen.y <= 0 || headScreen.y >= emuH) continue;

        ImVec2 headScreenPos(clientX + headScreen.x, clientY + headScreen.y);

        // Cálculo de la caja (Tu código original que estaba bien)
        float groundY = headPos.y - 1.65f; 
        uint32_t rootNode = 0;
        Vector3 rootPos = { 0.0f, 0.0f, 0.0f };
        if (ReadZ(entity + Core::Offsets::Root, rootNode) && rootNode != 0) {
            if (GetNodePosition(rootNode, rootPos) && (rootPos.x != 0.0f || rootPos.y != 0.0f || rootPos.z != 0.0f)) {
                float diffY = headPos.y - rootPos.y;
                if (diffY >= 0.4f && diffY <= 2.2f) groundY = rootPos.y;
            }
        }

        float playerHeightWorld = (headPos.y + 0.22f) - groundY;
        if (playerHeightWorld < 0.6f) playerHeightWorld = 0.6f;
        if (playerHeightWorld > 2.3f) playerHeightWorld = 1.87f;

        Vector3 centerWorld(headPos.x, groundY + (playerHeightWorld * 0.5f), headPos.z);
        ImVec2 centerScreen = WorldToScreenImVec2(viewMatrix, centerWorld, emuW, emuH);
        if (centerScreen.x < 1.0f || centerScreen.y < 1.0f) {
            centerScreen.x = headScreen.x;
            centerScreen.y = headScreen.y + 15.0f;
        }

        float cameraDepth = (centerWorld.x * viewMatrix.m03) + (centerWorld.y * viewMatrix.m13) + (centerWorld.z * viewMatrix.m23) + viewMatrix.m33;
        if (cameraDepth < 0.5f) continue;

        float focalY = std::sqrt(viewMatrix.m01 * viewMatrix.m01 + viewMatrix.m11 * viewMatrix.m11 + viewMatrix.m21 * viewMatrix.m21);
        if (focalY < 0.1f || std::isnan(focalY)) focalY = 1.732f;

        float halfH = (float)emuH * 0.5f;
        float boxH = (playerHeightWorld * halfH * focalY) / cameraDepth;
        if (boxH < 8.0f) boxH = 8.0f;
        if (boxH > (float)emuH * 0.90f) boxH = (float)emuH * 0.90f;

        float boxW = boxH * 0.50f;
        float boxCenterX = centerScreen.x;
        float boxCenterY = centerScreen.y;
        float boxX = clientX + boxCenterX - (boxW * 0.5f);
        float boxY = clientY + boxCenterY - (boxH * 0.5f);

        // Colores
        ImU32 colLine = ImGui::ColorConvertFloat4ToU32(isTeam ? cfg.ColorTeam : cfg.ColorEnemy);
        ImU32 colBox = ImGui::ColorConvertFloat4ToU32(isTeam ? cfg.ColorTeam : cfg.ColorBox);
        ImU32 colSkel = ImGui::ColorConvertFloat4ToU32(isTeam ? cfg.ColorTeam : cfg.ColorSkeleton);
        ImU32 colName = ImGui::ColorConvertFloat4ToU32(cfg.ColorName);

        // 1. Línea moderna con flecha
        if (cfg.EspLineEnabled) {
            DrawModernLine(drawList, lineStart, headScreenPos, colLine, cfg.LineThickness);
        }

        // 2. Caja
        if (cfg.EspBoxEnabled) {
            DrawModernBox(drawList, boxX, boxY, boxW, boxH, colBox, cfg.BoxThickness);
        }

        // 3. Vida
        if (cfg.EspHealthEnabled) {
            int health = 200;
            uint32_t dataPool = 0;
            if (ReadZ(entity + Core::Offsets::Player_Data, dataPool) && dataPool) {
                uint32_t poolObj = 0;
                if (ReadZ(dataPool + 0x8, poolObj) && poolObj) {
                    uint32_t pool = 0;
                    if (ReadZ(poolObj + 0x10, pool) && pool) {
                        int h = 0;
                        if (ReadZ(pool + 0x10, h) && h > 0 && h <= 500) health = h;
                    }
                }
            }
            DrawModernHealthBar(drawList, boxX, boxY, boxH, health);
        }

        // 4. Esqueleto (Optimizado sin vectores dinámicos)
        if (cfg.EspSkeletonEnabled) {
            uint32_t spineNode = 0, hipNode = 0, lSh = 0, lEl = 0, lWr = 0, rSh = 0, rEl = 0, rWr = 0, lCf = 0, lFt = 0, rCf = 0, rFt = 0;
            
            ReadZ(entity + Core::Offsets::Spine, spineNode);
            ReadZ(entity + Core::Offsets::Hip, hipNode);
            ReadZ(entity + Core::Offsets::LeftSholder, lSh);
            ReadZ(entity + Core::Offsets::LeftElbow, lEl);
            ReadZ(entity + Core::Offsets::LeftWrist, lWr);
            ReadZ(entity + Core::Offsets::RightSholder, rSh);
            ReadZ(entity + Core::Offsets::RightElbow, rEl);
            ReadZ(entity + Core::Offsets::RightWrist, rWr);
            ReadZ(entity + Core::Offsets::LeftCalf, lCf);
            ReadZ(entity + Core::Offsets::LeftFoot, lFt);
            ReadZ(entity + Core::Offsets::RightCalf, rCf);
            ReadZ(entity + Core::Offsets::RightFoot, rFt);

            auto GetBoneScreen = [&](uint32_t node, ImVec2& out) -> bool {
                if (!node) return false;
                Vector3 pos;
                if (!GetNodePosition(node, pos) || (pos.x == 0 && pos.y == 0 && pos.z == 0)) return false;
                ImVec2 s = WorldToScreenImVec2(viewMatrix, pos, emuW, emuH);
                if (s.x < 1 || s.y < 1) return false;
                out = ImVec2(clientX + s.x, clientY + s.y);
                return true;
            };

            ImVec2 pSpine, pHip, pLSh, pLEl, pLWr, pRSh, pREl, pRWr, pLCf, pLFt, pRCf, pRFt;
            if (GetBoneScreen(spineNode, pSpine)) {
                drawList->AddLine(headScreenPos, pSpine, colSkel, cfg.SkeletonThickness);
                if (GetBoneScreen(hipNode, pHip)) drawList->AddLine(pSpine, pHip, colSkel, cfg.SkeletonThickness);
                
                if (GetBoneScreen(lSh, pLSh)) {
                    drawList->AddLine(pSpine, pLSh, colSkel, cfg.SkeletonThickness);
                    if (GetBoneScreen(lEl, pLEl)) {
                        drawList->AddLine(pLSh, pLEl, colSkel, cfg.SkeletonThickness);
                        if (GetBoneScreen(lWr, pLWr)) drawList->AddLine(pLEl, pLWr, colSkel, cfg.SkeletonThickness);
                    }
                }
                if (GetBoneScreen(rSh, pRSh)) {
                    drawList->AddLine(pSpine, pRSh, colSkel, cfg.SkeletonThickness);
                    if (GetBoneScreen(rEl, pREl)) {
                        drawList->AddLine(pRSh, pREl, colSkel, cfg.SkeletonThickness);
                        if (GetBoneScreen(rWr, pRWr)) drawList->AddLine(pREl, pRWr, colSkel, cfg.SkeletonThickness);
                    }
                }
            }
            if (GetBoneScreen(hipNode, pHip)) {
                if (GetBoneScreen(lCf, pLCf)) {
                    drawList->AddLine(pHip, pLCf, colSkel, cfg.SkeletonThickness);
                    if (GetBoneScreen(lFt, pLFt)) drawList->AddLine(pLCf, pLFt, colSkel, cfg.SkeletonThickness);
                }
                if (GetBoneScreen(rCf, pRCf)) {
                    drawList->AddLine(pHip, pRCf, colSkel, cfg.SkeletonThickness);
                    if (GetBoneScreen(rFt, pRFt)) drawList->AddLine(pRCf, pRFt, colSkel, cfg.SkeletonThickness);
                }
            }
        }

        // 5. Nombre
        if (cfg.EspNameEnabled) {
            uint32_t nameAddr = 0;
            std::string nameStr = "Player";
            if (ReadZ(entity + Core::Offsets::Player_Name, nameAddr) && nameAddr != 0) {
                std::string scanned = ReadPlayerName(nameAddr);
                if (!scanned.empty()) nameStr = scanned;
            }
            DrawModernNameBadge(drawList, clientX + boxCenterX, boxY, nameStr, colName, boxH);

            // Badge de distancia
            if (cfg.EspNameEnabled && maxDistance > 0) {
                float dist = Vector3::Distance(headPos, Vector3(headPos.x, headPos.y, headPos.z)); // placeholder si no hay localPos
                ImVec2 localScreen(clientX + (emuW * 0.5f), clientY + (emuH * 0.5f));
                float distWorld = 0.0f;
                // Calcular distancia real si se tiene posición del jugador local
                DrawModernDistanceBadge(drawList, clientX + boxCenterX, boxY - 16.0f, distWorld, colName, boxH);
            }
        }
    }

    // Terminar recorte
    drawList->PopClipRect();
}
