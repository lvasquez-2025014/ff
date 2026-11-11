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
#include "Theme.h"
#include "../Bridge/AdbClient.h"

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

namespace UI {inline void RenderMenu() {
    static int activeTab = 0;
    const char* tabNames[] = {"ESP","ADB","Opciones"};
    const int tabCount = 3;
    ImGui::TextColored(ImVec4(0.11f, 0.55f, 0.98f, 1.0f), "// AIMBOT VISIBLE");
    ImGui::Dummy(ImVec2(0, 4));

    ToggleSwitch("Activar Aimbot", &Core::GetConfig().AimbotEnabled);
    if (Core::GetConfig().AimbotEnabled) {
        MinimalSlider("Radio FOV", &Core::GetConfig().AimbotFov, 50.0f, 500.0f, "%.0f");
        MinimalSlider("Suavidad", &Core::GetConfig().AimbotSmooth, 1.0f, 10.0f, "%.1f");
        ToggleSwitch("Dibujar FOV", &Core::GetConfig().AimbotDrawFov);
    }
    ImGui::Dummy(ImVec2(0, 8));
    ImGui::SetNextWindowSize(ImVec2(820, 520), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.85f);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

    if (ImGui::Begin("##modern_menu", nullptr, flags)) {
        ImVec2 pos = ImGui::GetWindowPos();
        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 winSize = ImGui::GetWindowSize();

        draw->AddRectFilled(pos, ImVec2(pos.x + winSize.x, pos.y + winSize.y), IM_COL32(15, 17, 23, 220), 14.0f);
        draw->AddRect(pos, ImVec2(pos.x + winSize.x, pos.y + winSize.y), IM_COL32(30, 35, 45, 150), 14.0f, 0, 1.0f);
        draw->AddRectFilled(pos, ImVec2(pos.x + winSize.x, pos.y + 3.0f), IM_COL32(17, 140, 248, 200), 14.0f, ImDrawFlags_RoundCornersTop);

        float sidebarW = 70.0f;
        draw->AddRectFilled(ImVec2(pos.x, pos.y + 3.0f), ImVec2(pos.x + sidebarW, pos.y + winSize.y), IM_COL32(20, 22, 28, 230), 0.0f, ImDrawFlags_RoundCornersBottomLeft);

        ImGui::SetCursorScreenPos(ImVec2(pos.x + 15.0f, pos.y + 15.0f));
        ImGui::TextColored(ImVec4(0.11f, 0.55f, 0.98f, 1.0f), "BR");
        ImGui::SameLine(0, 4);
        ImGui::TextColored(ImVec4(0.92f, 0.94f, 0.98f, 1.0f), "MODS");

        ImGui::Dummy(ImVec2(0, 30.0f));
        for (int i = 0; i < tabCount; i++) {
            bool isActive = (activeTab == i);
            ImVec2 btnPos = ImGui::GetCursorScreenPos();
            ImVec2 btnSize = ImVec2(40.0f, 40.0f);
            if (isActive) {
                draw->AddRectFilled(btnPos, ImVec2(btnPos.x + btnSize.x, btnPos.y + btnSize.y), IM_COL32(17, 140, 248, 40), 10.0f);
                draw->AddRect(btnPos, ImVec2(btnPos.x + btnSize.x, btnPos.y + btnSize.y), IM_COL32(17, 140, 248, 120), 10.0f, 0, 1.0f);
            }
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.11f, 0.55f, 0.98f, 0.10f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.11f, 0.55f, 0.98f, 0.20f));
            char tabId[32]; sprintf_s(tabId,"##tab_%d",i);
            if (ImGui::Button(tabId, btnSize)) activeTab = i;
            ImGui::PopStyleColor(3);
            ImVec2 center = ImVec2(btnPos.x + btnSize.x * 0.5f, btnPos.y + btnSize.y * 0.5f);
            ImU32 iconCol = isActive ? IM_COL32(17, 140, 248, 255) : IM_COL32(100, 110, 130, 200);
            draw->AddCircleFilled(center, 6.0f, iconCol);
            ImGui::Dummy(ImVec2(0, 12.0f));
        }
        draw->AddLine(ImVec2(pos.x + sidebarW, pos.y + 10.0f), ImVec2(pos.x + sidebarW, pos.y + winSize.y - 10.0f), IM_COL32(30, 35, 45, 100), 1.0f);

        ImVec2 contentMin = ImVec2(pos.x + sidebarW + 20.0f, pos.y + 20.0f);
        ImVec2 contentMax = ImVec2(pos.x + winSize.x - 20.0f, pos.y + winSize.y - 20.0f);

        ImGui::SetCursorScreenPos(ImVec2(contentMin.x, contentMin.y));
        ImGui::TextColored(ImVec4(0.11f, 0.55f, 0.98f, 1.0f), tabNames[activeTab]);
        ImGui::SameLine(0, 8);
        ImGui::TextColored(ImVec4(0.65f, 0.70f, 0.78f, 0.90f), "// Configuracion");

        ImGui::Dummy(ImVec2(0, 15.0f));

        if (activeTab == 0) {
            ImGui::BeginChild("##esp_content", ImVec2(contentMax.x - contentMin.x, 340.0f), true);
            {
                UI::DCM(contentMin, ImVec2(contentMax.x, contentMin.y + 320.0f), "VISUALES ESP", IM_COL32(17, 140, 248, 255));
                ImGui::SetCursorScreenPos(ImVec2(contentMin.x + 15.0f, contentMin.y + 45.0f));
                float innerW = contentMax.x - contentMin.x - 30.0f;
                float colW = innerW * 0.5f - 10.0f;

                ImGui::PushItemWidth(colW);
                UI::TSM("Activar ESP Líneas", &cfg.EspLineEnabled); ImGui::Dummy(ImVec2(0,12));
                UI::TSM("Caja (Box)", &cfg.EspBoxEnabled); ImGui::Dummy(ImVec2(0,12));
                UI::TSM("Esqueleto", &cfg.EspSkeletonEnabled); ImGui::Dummy(ImVec2(0,12));
                UI::TSM("Barra de Vida", &cfg.EspHealthEnabled); ImGui::PopItemWidth();

                ImGui::SameLine(contentMin.x + innerW * 0.5f + 10.0f);
                ImGui::PushItemWidth(colW);
                UI::TSM("Mostrar Nombres", &cfg.EspNameEnabled); ImGui::Dummy(ImVec2(0,12));
                UI::TSM("Mostrar Aliados", &cfg.ShowTeam); ImGui::Dummy(ImVec2(0,12));
                UI::MSM("Grosor de línea", &cfg.LineThickness, 0.5f, 5.0f, "%.1f px"); ImGui::Dummy(ImVec2(0,16));
                UI::MSM("Grosor de caja", &cfg.BoxThickness, 0.5f, 4.0f, "%.1f px"); ImGui::PopItemWidth();

                ImGui::Dummy(ImVec2(0, 12));
                ImGui::TextColored(ImVec4(0.65f, 0.70f, 0.78f, 0.90f), "Colores:");
                ImGui::SameLine(contentMin.x + 140.0f);
                ImGui::ColorEdit4("##col_enemy", (float*)&cfg.ColorEnemy, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoLabel);
                ImGui::SameLine(0, 20.0f);
                ImGui::ColorEdit4("##col_team", (float*)&cfg.ColorTeam, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoLabel);
                ImGui::EndChild();
            }
            ImGui::Dummy(ImVec2(0, 15.0f));

            ImGui::BeginChild("##adb_content", ImVec2(contentMax.x - contentMin.x, 150.0f), true);
            {
                UI::DCM(contentMin, ImVec2(contentMax.x, contentMin.y + 130.0f), "CONEXIÓN ADB", IM_COL32(17, 140, 248, 255));
                ImGui::SetCursorScreenPos(ImVec2(contentMin.x + 15.0f, contentMin.y + 45.0f));
                static bool prevAdbState = false;
                bool currAdbState = cfg.AdbConnected;
                UI::TSM("Activar ADB", &cfg.AdbConnected);
                if (!prevAdbState && currAdbState) Bridge::AdbClient::Get().StartConnectionAsync();
                prevAdbState = currAdbState;
                ImGui::SameLine(0, 15.0f);
                if (cfg.AdbConnecting) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Estado: CONECTANDO...");
                else if (cfg.AdbConnected) ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Estado: ONLINE");
                else ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Estado: DESCONECTADO");
                ImGui::EndChild();
            }
        }
        else if (activeTab == 1) {
            ImGui::BeginChild("##opt_content", ImVec2(contentMax.x - contentMin.x, 480.0f), true);
            {
                UI::DCM(contentMin, ImVec2(contentMax.x, contentMin.y + 100.0f), "GENERAL", IM_COL32(17, 140, 248, 255));
                ImGui::SetCursorScreenPos(ImVec2(contentMin.x + 15.0f, contentMin.y + 45.0f));
                UI::TSM("Clickeable", &cfg.IsClickable); ImGui::Dummy(ImVec2(0,12));
                UI::TSM("Verificado", &cfg.Verified);
                ImGui::Dummy(ImVec2(0, 20));
                UI::DCM(contentMin, ImVec2(contentMax.x, contentMin.y + 200.0f), "APARIENCIA", IM_COL32(17, 140, 248, 255));
                ImGui::SetCursorScreenPos(ImVec2(contentMin.x + 15.0f, contentMin.y + 145.0f));
                UI::MSM("Transparencia Menú", &cfg.TabAlpha, 0.5f, 1.0f, "%.2f");
                ImGui::Dummy(ImVec2(0, 20));
                UI::DCM(contentMin, ImVec2(contentMax.x, contentMin.y + 300.0f), "ZONA DE PELIGRO", IM_COL32(220, 60, 70, 255));
                ImGui::SetCursorScreenPos(ImVec2(contentMin.x + 15.0f, contentMin.y + 245.0f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.50f, 0.15f, 0.18f, 0.90f));
                if (ImGui::Button("Cerrar Todo", ImVec2(140, 34))) ExitProcess(0);
                ImGui::PopStyleColor();
                ImGui::EndChild();
            }
        }
    }
    ImGui::End();
}
} // namespace UI
