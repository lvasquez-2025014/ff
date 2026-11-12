#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <cstdio>
#include <algorithm>
#include "../Core/Config.h"
#include "../Core/Offsets.h"
#include "Theme.h"
#include "../Bridge/AdbClient.h"
#include "../Bridge/Process.h"

extern HWND hwnd;
extern ID3D11ShaderResourceView* logo;

namespace image2 {
    extern ID3D11ShaderResourceView* main;
    extern ID3D11ShaderResourceView* gless;
    extern ID3D11ShaderResourceView* help;
    extern ID3D11ShaderResourceView* settings;
    extern ID3D11ShaderResourceView* panel;
    extern ID3D11ShaderResourceView* other;
    extern ID3D11ShaderResourceView* save;
    extern ID3D11ShaderResourceView* login111;
    extern ID3D11ShaderResourceView* Register111;
    extern ID3D11ShaderResourceView* rayo;
    extern ID3D11ShaderResourceView* flecha;
    extern ID3D11ShaderResourceView* ancla;
    extern ID3D11ShaderResourceView* fastfire;
    extern ID3D11ShaderResourceView* tpenemy;
    extern ID3D11ShaderResourceView* nieve;
    extern ID3D11ShaderResourceView* ghost;
    extern ID3D11ShaderResourceView* ffimagen;
    extern ID3D11ShaderResourceView* ffimagenMAX;
    extern ID3D11ShaderResourceView* ffimagenTELA;
}

namespace UI {

// Botón de navegación lateral con icono, indicador activo y subtítulo
inline bool SidebarTabButton(const char* idStr, const char* label, const char* sublabel, ID3D11ShaderResourceView* iconTex, int tabIndex, bool isActive, float width) {
    ImGuiWindow* w = ImGui::GetCurrentWindow();
    if (w->SkipItems) return false;

    ImGuiID id = w->GetID(idStr);
    float h = 44.0f;
    ImVec2 pos = w->DC.CursorPos;
    ImRect bb(pos, ImVec2(pos.x + width, pos.y + h));
    ImGui::ItemSize(bb, 0.0f);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool hovered = false, held = false;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    ImDrawList* dl = w->DrawList;

    if (isActive) {
        dl->AddRectFilled(bb.Min, bb.Max, IM_COL32(36, 148, 255, 34), 9.0f);
        dl->AddRect(bb.Min, bb.Max, IM_COL32(36, 148, 255, 110), 9.0f, 0, 1.0f);
        dl->AddRectFilled(
            ImVec2(bb.Min.x, bb.Min.y + 8.0f),
            ImVec2(bb.Min.x + 3.5f, bb.Max.y - 8.0f),
            IM_COL32(36, 148, 255, 255),
            2.0f
        );
    } else if (hovered) {
        dl->AddRectFilled(bb.Min, bb.Max, IM_COL32(255, 255, 255, 10), 9.0f);
        dl->AddRect(bb.Min, bb.Max, IM_COL32(255, 255, 255, 22), 9.0f, 0, 1.0f);
    }

    // Icono o geometría vectorial
    ImVec2 iconCenter(bb.Min.x + 24.0f, bb.Min.y + h * 0.5f);
    ImU32 iconCol = isActive ? IM_COL32(55, 165, 255, 255) : (hovered ? IM_COL32(205, 215, 232, 255) : IM_COL32(130, 142, 168, 230));

    if (iconTex != nullptr) {
        dl->AddImage((ImTextureID)iconTex, ImVec2(iconCenter.x - 9.0f, iconCenter.y - 9.0f), ImVec2(iconCenter.x + 9.0f, iconCenter.y + 9.0f), ImVec2(0, 0), ImVec2(1, 1), iconCol);
    } else {
        if (tabIndex == 0) {
            dl->AddCircle(iconCenter, 7.0f, iconCol, 16, 1.5f);
            dl->AddCircleFilled(iconCenter, 2.5f, iconCol, 12);
        } else if (tabIndex == 1) {
            dl->AddRect(ImVec2(iconCenter.x - 7.0f, iconCenter.y - 5.0f), ImVec2(iconCenter.x + 7.0f, iconCenter.y + 5.0f), iconCol, 2.0f, 0, 1.5f);
            dl->AddCircleFilled(iconCenter, 2.0f, iconCol, 10);
        } else if (tabIndex == 2) {
            dl->AddCircle(iconCenter, 7.0f, iconCol, 16, 1.5f);
            dl->AddLine(ImVec2(iconCenter.x - 4.0f, iconCenter.y), ImVec2(iconCenter.x + 4.0f, iconCenter.y), iconCol, 1.5f);
        } else {
            dl->AddCircle(iconCenter, 6.5f, iconCol, 8, 1.5f);
        }
    }

    ImU32 titleCol = isActive ? IM_COL32(245, 248, 255, 255) : (hovered ? IM_COL32(220, 226, 240, 255) : IM_COL32(170, 180, 200, 255));
    ImU32 subCol   = isActive ? IM_COL32(95, 175, 255, 230)  : IM_COL32(105, 115, 138, 200);

    dl->AddText(ImVec2(bb.Min.x + 44.0f, bb.Min.y + 7.0f), titleCol, label);
    dl->AddText(ImVec2(bb.Min.x + 44.0f, bb.Min.y + 23.0f), subCol, sublabel);

    return pressed;
}

inline void BeginStyledCard(const char* id, const ImVec2& size) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16.0f, 14.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.075f, 0.086f, 0.122f, 0.94f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.15f, 0.18f, 0.26f, 0.85f));
    ImGui::BeginChild(id, size, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
}

inline void EndStyledCard() {
    ImGui::EndChild();
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(3);
}

inline void RenderMenu() {
    auto& cfg = Core::GetConfig();
    static int activeTab = 0;

    struct TabInfo {
        const char* id;
        const char* name;
        const char* desc;
        const char* headerTitle;
        const char* headerSub;
    };

    const TabInfo tabs[] = {
        { "##tab_combat",  "Combate",   "Aimbot & FOV",     "MÓDULO DE COMBATE",  "// Aimbot Visible & Precisión" },
        { "##tab_visuals", "Visuales",  "ESP 2D / 3D",      "MÓDULO DE VISUALES", "// Renderizado ESP en Tiempo Real" },
        { "##tab_bridge",  "Conexión",  "ADB & Memoria",    "PUENTE DE CONEXIÓN", "// Enlace ADB y Estado del Emulador" },
        { "##tab_system",  "Sistema",   "Ajustes & Panic",  "AJUSTES DE SISTEMA", "// Configuración General y Seguridad" }
    };
    const int tabCount = 4;

    ImGui::SetNextWindowSize(ImVec2(840.0f, 540.0f), ImGuiCond_Always);
    ImGui::SetNextWindowPos(
        ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
        ImGuiCond_Once,
        ImVec2(0.5f, 0.5f)
    );

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar;

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, std::clamp(cfg.TabAlpha, 0.5f, 1.0f));
    if (ImGui::Begin("##modern_menu", nullptr, flags)) {
        ImVec2 pos = ImGui::GetWindowPos();
        ImVec2 winSize = ImGui::GetWindowSize();
        ImDrawList* draw = ImGui::GetWindowDrawList();

        float sidebarW = 205.0f;

        // Sombra exterior suave y fondo principal redondeado
        draw->AddRectFilled(
            ImVec2(pos.x - 2.0f, pos.y - 2.0f),
            ImVec2(pos.x + winSize.x + 2.0f, pos.y + winSize.y + 2.0f),
            IM_COL32(0, 0, 0, 110),
            16.0f
        );
        draw->AddRectFilled(pos, ImVec2(pos.x + winSize.x, pos.y + winSize.y), IM_COL32(11, 13, 19, 246), 14.0f);

        // Fondo de la barra lateral izquierda
        draw->AddRectFilled(
            pos,
            ImVec2(pos.x + sidebarW, pos.y + winSize.y),
            IM_COL32(15, 18, 26, 252),
            14.0f,
            ImDrawFlags_RoundCornersLeft
        );

        // Línea divisoria vertical de la barra lateral
        draw->AddLine(
            ImVec2(pos.x + sidebarW, pos.y + 1.0f),
            ImVec2(pos.x + sidebarW, pos.y + winSize.y - 1.0f),
            IM_COL32(34, 41, 58, 200),
            1.0f
        );

        // Barra superior luminosa de acento
        draw->AddRectFilled(
            ImVec2(pos.x + 1.0f, pos.y + 1.0f),
            ImVec2(pos.x + winSize.x - 1.0f, pos.y + 3.5f),
            IM_COL32(36, 148, 255, 235),
            14.0f,
            ImDrawFlags_RoundCornersTop
        );

        // Borde exterior nítido
        draw->AddRect(pos, ImVec2(pos.x + winSize.x, pos.y + winSize.y), IM_COL32(42, 50, 72, 190), 14.0f, 0, 1.0f);

        // ═════════════════════════════════════════════════════════════════════
        // BARRA LATERAL (BRANDING + TABS + ESTADO EN VIVO)
        // ═════════════════════════════════════════════════════════════════════
        ImVec2 brandPos(pos.x + 18.0f, pos.y + 20.0f);
        if (logo != nullptr) {
            draw->AddImage((ImTextureID)logo, brandPos, ImVec2(brandPos.x + 34.0f, brandPos.y + 34.0f));
        } else {
            draw->AddRectFilled(brandPos, ImVec2(brandPos.x + 34.0f, brandPos.y + 34.0f), IM_COL32(36, 148, 255, 40), 8.0f);
            draw->AddRect(brandPos, ImVec2(brandPos.x + 34.0f, brandPos.y + 34.0f), IM_COL32(36, 148, 255, 180), 8.0f, 0, 1.2f);
            draw->AddText(ImVec2(brandPos.x + 8.0f, brandPos.y + 9.0f), IM_COL32(55, 165, 255, 255), "BR");
        }

        draw->AddText(ImVec2(brandPos.x + 44.0f, brandPos.y + 2.0f), IM_COL32(245, 248, 255, 255), "BR MODS");
        draw->AddText(ImVec2(brandPos.x + 44.0f, brandPos.y + 18.0f), IM_COL32(55, 165, 255, 230), "EXTERNAL OVERLAY");

        draw->AddLine(
            ImVec2(pos.x + 16.0f, pos.y + 68.0f),
            ImVec2(pos.x + sidebarW - 16.0f, pos.y + 68.0f),
            IM_COL32(36, 43, 62, 160),
            1.0f
        );

        draw->AddText(ImVec2(pos.x + 18.0f, pos.y + 80.0f), IM_COL32(100, 112, 138, 220), "NAVEGACIÓN");

        ID3D11ShaderResourceView* tabIcons[4] = {
            image2::main,
            image2::gless,
            image2::panel,
            image2::settings
        };

        float navStartY = pos.y + 102.0f;
        for (int i = 0; i < tabCount; i++) {
            ImGui::SetCursorScreenPos(ImVec2(pos.x + 12.0f, navStartY + i * 52.0f));
            if (SidebarTabButton(tabs[i].id, tabs[i].name, tabs[i].desc, tabIcons[i], i, activeTab == i, sidebarW - 24.0f)) {
                activeTab = i;
            }
        }

        // Tarjeta inferior de estado en la barra lateral
        ImVec2 statusBoxMin(pos.x + 12.0f, pos.y + winSize.y - 78.0f);
        ImVec2 statusBoxMax(pos.x + sidebarW - 12.0f, pos.y + winSize.y - 14.0f);
        draw->AddRectFilled(statusBoxMin, statusBoxMax, IM_COL32(19, 23, 34, 240), 9.0f);
        draw->AddRect(statusBoxMin, statusBoxMax, IM_COL32(38, 46, 66, 170), 9.0f, 0, 1.0f);

        ImU32 dotCol = cfg.AdbConnecting
            ? IM_COL32(255, 195, 40, 255)
            : (cfg.AdbConnected ? IM_COL32(45, 225, 125, 255) : IM_COL32(235, 80, 90, 255));
        const char* adbText = cfg.AdbConnecting
            ? "ADB: CONECTANDO..."
            : (cfg.AdbConnected ? "ADB: CONECTADO" : "ADB: DESCONECTADO");

        draw->AddCircleFilled(ImVec2(statusBoxMin.x + 14.0f, statusBoxMin.y + 19.0f), 4.5f, dotCol, 16);
        draw->AddText(ImVec2(statusBoxMin.x + 25.0f, statusBoxMin.y + 11.0f), IM_COL32(230, 236, 248, 255), adbText);

        char fpsBuf[64];
        sprintf_s(fpsBuf, "FPS: %.0f  |  [INS] Menú", ImGui::GetIO().Framerate);
        draw->AddText(ImVec2(statusBoxMin.x + 14.0f, statusBoxMin.y + 36.0f), IM_COL32(125, 136, 162, 230), fpsBuf);

        // ═════════════════════════════════════════════════════════════════════
        // ENCABEZADO DEL ÁREA DE CONTENIDO
        // ═════════════════════════════════════════════════════════════════════
        float contentX = pos.x + sidebarW + 20.0f;
        float contentY = pos.y + 18.0f;
        float contentW = winSize.x - sidebarW - 40.0f;

        draw->AddText(ImVec2(contentX, contentY), IM_COL32(55, 165, 255, 255), tabs[activeTab].headerTitle);
        ImVec2 hTitleSz = ImGui::CalcTextSize(tabs[activeTab].headerTitle);
        draw->AddText(ImVec2(contentX + hTitleSz.x + 10.0f, contentY), IM_COL32(130, 142, 168, 230), tabs[activeTab].headerSub);

        // Píldora rápida de estado a la derecha del encabezado
        const char* modeBadge = cfg.IsClickable ? "MODO: INTERACTIVO" : "MODO: OVERLAY";
        ImVec2 badgeSz = ImGui::CalcTextSize(modeBadge);
        ImVec2 badgeMin(contentX + contentW - badgeSz.x - 20.0f, contentY - 3.0f);
        ImVec2 badgeMax(contentX + contentW, contentY + 19.0f);
        draw->AddRectFilled(badgeMin, badgeMax, IM_COL32(36, 148, 255, 28), 6.0f);
        draw->AddRect(badgeMin, badgeMax, IM_COL32(36, 148, 255, 95), 6.0f, 0, 1.0f);
        draw->AddText(ImVec2(badgeMin.x + 10.0f, badgeMin.y + 3.0f), IM_COL32(115, 190, 255, 255), modeBadge);

        draw->AddLine(
            ImVec2(contentX, contentY + 26.0f),
            ImVec2(contentX + contentW, contentY + 26.0f),
            IM_COL32(34, 41, 58, 180),
            1.0f
        );

        float bodyY = contentY + 38.0f;
        float bodyH = winSize.y - (bodyY - pos.y) - 18.0f;
        float colGap = 14.0f;
        float colW = (contentW - colGap) * 0.5f;

        // ═════════════════════════════════════════════════════════════════════
        // TAB 0: COMBATE (AIMBOT VISIBLE)
        // ═════════════════════════════════════════════════════════════════════
        if (activeTab == 0) {
            ImGui::SetCursorScreenPos(ImVec2(contentX, bodyY));
            BeginStyledCard("##card_aimbot_main", ImVec2(colW, bodyH));
            {
                UI::CardHeader("AIMBOT VISIBLE", "Control Principal");

                UI::TSM("Activar Aimbot", &cfg.AimbotEnabled);
                UI::TSM("Dibujar Círculo FOV", &cfg.AimbotDrawFov);
                UI::TSM("Apuntar a Aliados", &cfg.ShowTeam);

                ImGui::Dummy(ImVec2(0.0f, 8.0f));
                ImGui::Separator();
                ImGui::Dummy(ImVec2(0.0f, 6.0f));

                ImGui::TextColored(ImVec4(0.70f, 0.75f, 0.85f, 1.0f), "Tecla de Activación:");
                ImGui::Dummy(ImVec2(0.0f, 2.0f));

                struct KeyOption { const char* name; int vk; };
                const KeyOption keyOpts[] = {
                    { "Click Derecho (RButton)", 0x02 },
                    { "Click Izquierdo (LButton)", 0x01 },
                    { "Mouse Lateral 4 (X1)", 0x05 },
                    { "Mouse Lateral 5 (X2)", 0x06 },
                    { "Tecla Shift", VK_SHIFT },
                    { "Tecla Alt", VK_MENU }
                };
                int currentKeyIdx = 0;
                for (int k = 0; k < 6; k++) {
                    if (cfg.AimbotKey == keyOpts[k].vk) {
                        currentKeyIdx = k;
                        break;
                    }
                }
                const char* keyNames[] = {
                    "Click Derecho (RButton)",
                    "Click Izquierdo (LButton)",
                    "Mouse Lateral 4 (X1)",
                    "Mouse Lateral 5 (X2)",
                    "Tecla Shift",
                    "Tecla Alt"
                };
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                if (ImGui::Combo("##aimbot_key", &currentKeyIdx, keyNames, IM_ARRAYSIZE(keyNames))) {
                    cfg.AimbotKey = keyOpts[currentKeyIdx].vk;
                }

                ImGui::Dummy(ImVec2(0.0f, 10.0f));
                ImGui::TextColored(ImVec4(0.50f, 0.56f, 0.68f, 0.95f), "Nota: Mantén presionada la tecla elegida\npara girar suavemente hacia la cabeza.");
            }
            EndStyledCard();

            ImGui::SetCursorScreenPos(ImVec2(contentX + colW + colGap, bodyY));
            BeginStyledCard("##card_aimbot_tuning", ImVec2(colW, bodyH));
            {
                UI::CardHeader("AJUSTES DE PRECISIÓN", "FOV & Suavizado");

                UI::MSM("Radio de FOV", &cfg.AimbotFov, 20.0f, 400.0f, "%.0f px");
                ImGui::Dummy(ImVec2(0.0f, 6.0f));
                UI::MSM("Suavizado (Smooth)", &cfg.AimbotSmooth, 1.0f, 10.0f, "%.1f");

                ImGui::Dummy(ImVec2(0.0f, 12.0f));
                ImGui::Separator();
                ImGui::Dummy(ImVec2(0.0f, 8.0f));

                ImGui::TextColored(ImVec4(0.88f, 0.92f, 0.98f, 1.0f), "Guía de Valores:");
                ImGui::Dummy(ImVec2(0.0f, 4.0f));
                ImGui::TextColored(ImVec4(0.58f, 0.64f, 0.76f, 1.0f), "• Smooth 1.0 - 2.5: Rápido / Agresivo");
                ImGui::TextColored(ImVec4(0.58f, 0.64f, 0.76f, 1.0f), "• Smooth 3.0 - 6.0: Legit / Natural");
                ImGui::TextColored(ImVec4(0.58f, 0.64f, 0.76f, 1.0f), "• Smooth 7.0 - 10.0: Muy Suave");
            }
            EndStyledCard();

            ImGui::SetCursorScreenPos(ImVec2(contentX, bodyY + bodyH + colGap));
            BeginStyledCard("##card_aimbot_rage", ImVec2(colW, bodyH));
            {
                UI::CardHeader("AIMBOT RAGE", "Modo Continuo (estilo AimbotRage.cs)");

                UI::TSM("Activar Aimbot Rage", &cfg.AimBotRage);
                UI::TSM("Ignorar Noqueados", &cfg.AimBotIgnoreKnocked);
                UI::TSM("Apuntar a Aliados", &cfg.ShowTeam);

                ImGui::Dummy(ImVec2(0.0f, 8.0f));
                ImGui::Separator();
                ImGui::Dummy(ImVec2(0.0f, 6.0f));

                ImGui::TextColored(ImVec4(0.70f, 0.75f, 0.85f, 1.0f), "Modo de Seleccion:");
                ImGui::Dummy(ImVec2(0.0f, 2.0f));
                const char* targetModes[] = { "Cerca de Mira", "360 Grados", "Vida Mas Baja", "Cerca del Jugador" };
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::Combo("##rage_target_mode", &cfg.AimBotTargetMode, targetModes, IM_ARRAYSIZE(targetModes));

                ImGui::Dummy(ImVec2(0.0f, 10.0f));
                UI::MSM("Distancia Maxima", &cfg.AimBotMaxDistance, 50.0f, 1000.0f, "%.0f m");
                ImGui::Dummy(ImVec2(0.0f, 6.0f));
                UI::MSM("FOV del Rage", &cfg.AimBotFov, 20.0f, 400.0f, "%.0f px");
                ImGui::Dummy(ImVec2(0.0f, 6.0f));
                //UI::MSM("Sleep entre Aim (ms)", &cfg.AimBotSmoothMs, 1, 50, "%d ms");

                ImGui::Dummy(ImVec2(0.0f, 10.0f));
                ImGui::TextColored(ImVec4(0.88f, 0.60f, 0.60f, 0.95f), "ADVERTENCIA: El modo Rage escribe rotacion\ndirectamente en memoria continuamente.");
            }
            EndStyledCard();
        }
        // ══════════════════════════════════════════════════════════════════════
        // TAB 1: VISUALES (ESP)
        // ═════════════════════════════════════════════════════════════════════
        else if (activeTab == 1) {
            ImGui::SetCursorScreenPos(ImVec2(contentX, bodyY));
            BeginStyledCard("##card_esp_toggles", ImVec2(colW, bodyH));
            {
                UI::CardHeader("ELEMENTOS ESP", "Sensores Visuales");

                UI::TSM("ESP Líneas (Snaplines)", &cfg.EspLineEnabled);
                UI::TSM("Caja 2D (Corner / Box)", &cfg.EspBoxEnabled);
                UI::TSM("Caja 3D (Cubo Wireframe)", &cfg.EspBox3dEnabled);
                UI::TSM("Esqueleto (Bones)", &cfg.EspSkeletonEnabled);
                UI::TSM("Barra de Vida (Health)", &cfg.EspHealthEnabled);
                UI::TSM("Mostrar Nombres", &cfg.EspNameEnabled);
                UI::TSM("Mostrar Aliados (Team)", &cfg.ShowTeam);
            }
            EndStyledCard();

            ImGui::SetCursorScreenPos(ImVec2(contentX + colW + colGap, bodyY));
            BeginStyledCard("##card_esp_style", ImVec2(colW, bodyH));
            {
                UI::CardHeader("ESTILO Y COLORES", "Personalización");

                UI::MSM("Grosor de Línea", &cfg.LineThickness, 0.5f, 5.0f, "%.1f px");
                ImGui::Dummy(ImVec2(0.0f, 4.0f));
                UI::MSM("Grosor de Caja", &cfg.BoxThickness, 0.5f, 4.0f, "%.1f px");
                ImGui::Dummy(ImVec2(0.0f, 4.0f));
                UI::MSM("Grosor de Esqueleto", &cfg.SkeletonThickness, 0.5f, 4.0f, "%.1f px");

                ImGui::Dummy(ImVec2(0.0f, 8.0f));
                ImGui::Separator();
                ImGui::Dummy(ImVec2(0.0f, 6.0f));

                UI::ColorRow("Color Enemigos", "##col_enemy", &cfg.ColorEnemy);
                UI::ColorRow("Color Aliados", "##col_team", &cfg.ColorTeam);
                UI::ColorRow("Color Cajas 2D/3D", "##col_box", &cfg.ColorBox);
                UI::ColorRow("Color Esqueleto", "##col_skel", &cfg.ColorSkeleton);
                UI::ColorRow("Color Nombres", "##col_name", &cfg.ColorName);
            }
            EndStyledCard();
        }
        // ═════════════════════════════════════════════════════════════════════
        // TAB 2: CONEXIÓN (ADB & MEMORIA)
        // ═════════════════════════════════════════════════════════════════════
        else if (activeTab == 2) {
            ImGui::SetCursorScreenPos(ImVec2(contentX, bodyY));
            BeginStyledCard("##card_adb_bridge", ImVec2(colW, bodyH));
            {
                UI::CardHeader("CONEXIÓN ADB", "Puente con Emulador");

                static bool prevAdbState = false;
                bool currAdbState = cfg.AdbConnected;
                if (UI::TSM("Activar Puente ADB", &cfg.AdbConnected)) {
                    currAdbState = cfg.AdbConnected;
                }
                if (!prevAdbState && currAdbState) {
                    Bridge::AdbClient::Get().StartConnectionAsync();
                }
                prevAdbState = currAdbState;

                ImGui::Dummy(ImVec2(0.0f, 10.0f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.14f, 0.52f, 0.95f, 0.85f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.20f, 0.60f, 1.00f, 1.00f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.10f, 0.45f, 0.85f, 1.00f));
                if (ImGui::Button("Conectar / Reconectar ADB [F1]", ImVec2(ImGui::GetContentRegionAvail().x, 34.0f))) {
                    Bridge::AdbClient::Get().StartConnectionAsync();
                }
                ImGui::PopStyleColor(3);

                ImGui::Dummy(ImVec2(0.0f, 12.0f));
                ImGui::Separator();
                ImGui::Dummy(ImVec2(0.0f, 8.0f));

                ImGui::TextColored(ImVec4(0.70f, 0.75f, 0.85f, 1.0f), "Estado de Enlace:");
                if (cfg.AdbConnecting) {
                    ImGui::TextColored(ImVec4(1.0f, 0.80f, 0.20f, 1.0f), "● CONECTANDO CON EMULADOR...");
                } else if (cfg.AdbConnected) {
                    ImGui::TextColored(ImVec4(0.18f, 0.90f, 0.50f, 1.0f), "● ONLINE (Sincronizado)");
                } else {
                    ImGui::TextColored(ImVec4(0.92f, 0.35f, 0.40f, 1.0f), "● DESCONECTADO");
                }

                ImGui::Dummy(ImVec2(0.0f, 6.0f));
                ImGui::TextColored(ImVec4(0.55f, 0.62f, 0.74f, 1.0f), "Mensaje: %s", cfg.AdbStatus.c_str());
            }
            EndStyledCard();

            ImGui::SetCursorScreenPos(ImVec2(contentX + colW + colGap, bodyY));
            BeginStyledCard("##card_telemetry", ImVec2(colW, bodyH));
            {
                UI::CardHeader("TELEMETRÍA DE MEMORIA", "Diagnóstico");

                uintptr_t il2cppVal = Core::Offsets::Il2Cpp != 0 ? Core::Offsets::Il2Cpp : Il2Cpp;
                ImGui::TextColored(ImVec4(0.70f, 0.75f, 0.85f, 1.0f), "Base Il2Cpp:");
                if (il2cppVal != 0) {
                    ImGui::TextColored(ImVec4(0.25f, 0.88f, 0.55f, 1.0f), "0x%llX", (unsigned long long)il2cppVal);
                } else {
                    ImGui::TextColored(ImVec4(0.65f, 0.70f, 0.78f, 0.80f), "Esperando lectura (0x0)...");
                }

                ImGui::Dummy(ImVec2(0.0f, 8.0f));
                ImGui::TextColored(ImVec4(0.70f, 0.75f, 0.85f, 1.0f), "Ventana de Renderizado:");
                if (RenderWindow && IsWindow(RenderWindow)) {
                    ImGui::TextColored(ImVec4(0.25f, 0.88f, 0.55f, 1.0f), "Detectada (HWND: 0x%p)", (void*)RenderWindow);
                } else {
                    ImGui::TextColored(ImVec4(0.95f, 0.65f, 0.25f, 1.0f), "No detectada aún");
                }

                ImGui::Dummy(ImVec2(0.0f, 12.0f));
                ImGui::Separator();
                ImGui::Dummy(ImVec2(0.0f, 8.0f));
                ImGui::TextColored(ImVec4(0.55f, 0.62f, 0.74f, 1.0f), "Atajos Rápidos:");
                ImGui::TextColored(ImVec4(0.78f, 0.83f, 0.92f, 1.0f), "• [F1] Iniciar conexión ADB");
                ImGui::TextColored(ImVec4(0.78f, 0.83f, 0.92f, 1.0f), "• [INSERT / HOME] Mostrar u ocultar menú");
                ImGui::TextColored(ImVec4(0.78f, 0.83f, 0.92f, 1.0f), "• [DELETE] Cerrar todo de emergencia");
            }
            EndStyledCard();
        }
        // ═════════════════════════════════════════════════════════════════════
        // TAB 3: SISTEMA (AJUSTES & PANIC)
        // ═════════════════════════════════════════════════════════════════════
        else if (activeTab == 3) {
            ImGui::SetCursorScreenPos(ImVec2(contentX, bodyY));
            BeginStyledCard("##card_sys_general", ImVec2(colW, bodyH));
            {
                UI::CardHeader("GENERAL Y APARIENCIA", "Preferencias");

                if (UI::TSM("Menú Clickeable", &cfg.IsClickable)) {
                    UI::ToggleClickability(hwnd, cfg.IsClickable);
                }
                UI::TSM("Estado Verificado", &cfg.Verified);

                ImGui::Dummy(ImVec2(0.0f, 8.0f));
                ImGui::Separator();
                ImGui::Dummy(ImVec2(0.0f, 6.0f));

                UI::MSM("Opacidad del Menú", &cfg.TabAlpha, 0.55f, 1.0f, "%.2f");
            }
            EndStyledCard();

            ImGui::SetCursorScreenPos(ImVec2(contentX + colW + colGap, bodyY));
            BeginStyledCard("##card_sys_danger", ImVec2(colW, bodyH));
            {
                UI::CardHeader("ZONA DE PELIGRO", "Seguridad", IM_COL32(235, 65, 75, 255));

                ImGui::TextColored(ImVec4(0.72f, 0.76f, 0.85f, 1.0f), "Acciones de cierre rápido:");
                ImGui::Dummy(ImVec2(0.0f, 8.0f));

                if (ImGui::Button("Cerrar Solo Overlay", ImVec2(ImGui::GetContentRegionAvail().x, 34.0f))) {
                    ExitProcess(0);
                }

                ImGui::Dummy(ImVec2(0.0f, 8.0f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.62f, 0.14f, 0.18f, 0.92f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.78f, 0.18f, 0.22f, 1.00f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.50f, 0.10f, 0.14f, 1.00f));
                if (ImGui::Button("PANIC: Cerrar Emulador y Todo [DEL]", ImVec2(ImGui::GetContentRegionAvail().x, 36.0f))) {
                    Bridge::TerminateAllBlueStacksProcesses();
                    ExitProcess(0);
                }
                ImGui::PopStyleColor(3);
            }
            EndStyledCard();
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

} // namespace UI
