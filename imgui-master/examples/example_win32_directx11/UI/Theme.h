#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <algorithm>
#include <cmath>
#include <string>
#include "../Core/Config.h"

namespace UI {
    // Paleta moderna Dark Cyber / Glassmorphism
    inline ImVec4 aB  = ImVec4(0.14f, 0.58f, 1.00f, 1.00f); // Accent Blue (#2494FF)
    inline ImVec4 aBH = ImVec4(0.28f, 0.68f, 1.00f, 1.00f); // Accent Hover
    inline ImVec4 bgM = ImVec4(0.043f, 0.051f, 0.075f, 0.96f); // Main Window BG (#0B0D13)
    inline ImVec4 bgS = ImVec4(0.059f, 0.067f, 0.098f, 0.98f); // Sidebar BG (#0F1119)
    inline ImVec4 bgC = ImVec4(0.075f, 0.086f, 0.122f, 0.92f); // Card BG (#13161F)
    inline ImVec4 bc  = ImVec4(0.14f, 0.17f, 0.24f, 0.70f); // Border Subtle
    inline ImVec4 tm  = ImVec4(0.95f, 0.96f, 0.99f, 1.00f); // Text Main
    inline ImVec4 ts  = ImVec4(0.68f, 0.73f, 0.82f, 1.00f); // Text Secondary
    inline ImVec4 td  = ImVec4(0.42f, 0.47f, 0.56f, 0.90f); // Text Muted
    inline ImVec4 hc  = ImVec4(0.14f, 0.58f, 1.00f, 0.14f);
    inline ImVec4 ac  = ImVec4(0.14f, 0.58f, 1.00f, 0.24f);
    inline ImVec4 fb  = ImVec4(0.10f, 0.12f, 0.17f, 0.90f);
    inline ImVec4 fh  = ImVec4(0.14f, 0.17f, 0.24f, 0.95f);
    inline ImVec4 fa  = ImVec4(0.14f, 0.58f, 1.00f, 0.85f);
    inline ImVec4 sg  = ImVec4(0.14f, 0.58f, 1.00f, 1.00f);
    inline ImVec4 sga = ImVec4(0.30f, 0.70f, 1.00f, 1.00f);

    inline void SetupImGuiStyle() {
        ImGuiStyle& s = ImGui::GetStyle();
        s.Alpha = 1.0f;
        s.DisabledAlpha = 0.45f;
        s.WindowPadding = ImVec2(0, 0);
        s.WindowRounding = 14.0f;
        s.WindowBorderSize = 0.0f;
        s.ChildRounding = 10.0f;
        s.ChildBorderSize = 1.0f;
        s.PopupRounding = 8.0f;
        s.PopupBorderSize = 1.0f;
        s.FramePadding = ImVec2(10, 6);
        s.FrameRounding = 6.0f;
        s.FrameBorderSize = 1.0f;
        s.ItemSpacing = ImVec2(10, 10);
        s.ItemInnerSpacing = ImVec2(8, 6);
        s.ScrollbarSize = 8.0f;
        s.ScrollbarRounding = 6.0f;
        s.GrabMinSize = 10.0f;
        s.GrabRounding = 6.0f;
        s.TabRounding = 8.0f;
        s.ButtonTextAlign = ImVec2(0.5f, 0.5f);

        s.Colors[ImGuiCol_Text] = tm;
        s.Colors[ImGuiCol_TextDisabled] = td;
        s.Colors[ImGuiCol_WindowBg] = bgM;
        s.Colors[ImGuiCol_ChildBg] = bgC;
        s.Colors[ImGuiCol_PopupBg] = ImVec4(0.06f, 0.07f, 0.10f, 0.98f);
        s.Colors[ImGuiCol_Border] = bc;
        s.Colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);
        s.Colors[ImGuiCol_FrameBg] = fb;
        s.Colors[ImGuiCol_FrameBgHovered] = fh;
        s.Colors[ImGuiCol_FrameBgActive] = fa;
        s.Colors[ImGuiCol_TitleBg] = ImVec4(0, 0, 0, 0);
        s.Colors[ImGuiCol_TitleBgActive] = ImVec4(0, 0, 0, 0);
        s.Colors[ImGuiCol_Button] = ImVec4(0.12f, 0.15f, 0.22f, 0.90f);
        s.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.18f, 0.23f, 0.33f, 1.00f);
        s.Colors[ImGuiCol_ButtonActive] = ac;
        s.Colors[ImGuiCol_CheckMark] = aB;
        s.Colors[ImGuiCol_SliderGrab] = sg;
        s.Colors[ImGuiCol_SliderGrabActive] = sga;
        s.Colors[ImGuiCol_Header] = fb;
        s.Colors[ImGuiCol_HeaderHovered] = hc;
        s.Colors[ImGuiCol_HeaderActive] = ac;
        s.Colors[ImGuiCol_Separator] = bc;
        s.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.58f, 1.00f, 0.50f);
        s.Colors[ImGuiCol_SeparatorActive] = aB;
        s.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.04f, 0.05f, 0.07f, 0.40f);
        s.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.24f, 0.34f, 0.80f);
        s.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.26f, 0.32f, 0.45f, 0.95f);
        s.Colors[ImGuiCol_ScrollbarGrabActive] = aB;
    }

    inline void ToggleClickability(HWND h, bool c) {
        if (!h) return;
        LONG_PTR e = GetWindowLongPtr(h, GWL_EXSTYLE);
        e = c ? (e & ~WS_EX_TRANSPARENT) : (e | WS_EX_TRANSPARENT);
        SetWindowLongPtr(h, GWL_EXSTYLE, e);
        SetLayeredWindowAttributes(h, RGB(0, 0, 0), 255, LWA_ALPHA);
        RedrawWindow(h, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME);
    }

    // Interruptor (Toggle Switch) alineado al ancho disponible de su contenedor
    inline bool TSM(const char* label, bool* v) {
        ImGuiWindow* w = ImGui::GetCurrentWindow();
        if (w->SkipItems) return false;

        ImGuiID id = w->GetID(label);
        float availW = ImGui::GetContentRegionAvail().x;
        float rowH = 24.0f;
        float swW = 38.0f;
        float swH = 20.0f;

        ImVec2 pos = w->DC.CursorPos;
        ImRect totalBb(pos, ImVec2(pos.x + availW, pos.y + rowH));
        ImGui::ItemSize(totalBb, 0.0f);
        if (!ImGui::ItemAdd(totalBb, id)) return false;

        bool hovered = false, held = false;
        bool pressed = ImGui::ButtonBehavior(totalBb, id, &hovered, &held);
        if (pressed) {
            *v = !(*v);
            ImGui::MarkItemEdited(id);
        }

        ImDrawList* dl = w->DrawList;

        // Fondo sutil al pasar el mouse por la fila
        if (hovered) {
            dl->AddRectFilled(
                ImVec2(totalBb.Min.x - 4.0f, totalBb.Min.y - 2.0f),
                ImVec2(totalBb.Max.x + 4.0f, totalBb.Max.y + 2.0f),
                IM_COL32(255, 255, 255, 8), 6.0f
            );
        }

        // Texto a la izquierda
        ImVec2 textSize = ImGui::CalcTextSize(label, nullptr, true);
        float textY = pos.y + (rowH - textSize.y) * 0.5f;
        ImU32 textCol = *v ? IM_COL32(242, 245, 252, 255) : (hovered ? IM_COL32(210, 218, 232, 255) : IM_COL32(168, 178, 198, 240));
        dl->AddText(ImVec2(pos.x, textY), textCol, label, ImGui::FindRenderedTextEnd(label));

        // Switch a la derecha
        float swX = pos.x + availW - swW;
        float swY = pos.y + (rowH - swH) * 0.5f;
        ImRect swBb(ImVec2(swX, swY), ImVec2(swX + swW, swY + swH));

        ImU32 trackCol = *v
            ? (hovered ? IM_COL32(55, 160, 255, 255) : IM_COL32(36, 148, 255, 240))
            : (hovered ? IM_COL32(42, 48, 64, 255) : IM_COL32(28, 33, 46, 240));
        ImU32 borderCol = *v
            ? IM_COL32(90, 185, 255, 180)
            : IM_COL32(65, 75, 98, 140);

        dl->AddRectFilled(swBb.Min, swBb.Max, trackCol, swH * 0.5f);
        dl->AddRect(swBb.Min, swBb.Max, borderCol, swH * 0.5f, 0, 1.0f);

        float knobR = (swH * 0.5f) - 3.0f;
        float knobX = *v ? (swBb.Max.x - knobR - 3.0f) : (swBb.Min.x + knobR + 3.0f);
        float knobY = swBb.Min.y + swH * 0.5f;

        if (*v) {
            dl->AddCircleFilled(ImVec2(knobX, knobY), knobR + 2.0f, IM_COL32(255, 255, 255, 45), 20);
        }
        dl->AddCircleFilled(
            ImVec2(knobX, knobY),
            knobR,
            *v ? IM_COL32(255, 255, 255, 255) : IM_COL32(150, 160, 182, 255),
            20
        );

        return pressed;
    }

    // Slider moderno con etiqueta superior y valor alineado a la derecha
    inline bool MSM(const char* label, float* v, float mn, float mx, const char* fmt = "%.1f") {
        ImGuiWindow* w = ImGui::GetCurrentWindow();
        if (w->SkipItems) return false;

        ImGuiID id = w->GetID(label);
        float availW = ImGui::GetContentRegionAvail().x;
        float topH = 18.0f;
        float trackAreaH = 16.0f;
        float totalH = topH + trackAreaH + 2.0f;

        ImVec2 pos = w->DC.CursorPos;
        ImRect totalBb(pos, ImVec2(pos.x + availW, pos.y + totalH));
        ImGui::ItemSize(totalBb, 0.0f);
        if (!ImGui::ItemAdd(totalBb, id)) return false;

        bool hovered = false, held = false;
        ImGui::ButtonBehavior(totalBb, id, &hovered, &held);

        float kr = 6.0f;
        if (held) {
            float mouseX = ImGui::GetIO().MousePos.x;
            float norm = std::clamp((mouseX - (pos.x + kr)) / std::max(1.0f, availW - 2.0f * kr), 0.0f, 1.0f);
            *v = mn + norm * (mx - mn);
            ImGui::MarkItemEdited(id);
        }

        ImDrawList* dl = w->DrawList;

        // Etiqueta a la izquierda y valor formateado a la derecha
        dl->AddText(pos, IM_COL32(178, 188, 208, 255), label, ImGui::FindRenderedTextEnd(label));

        char valBuf[48];
        ImFormatString(valBuf, sizeof(valBuf), fmt, *v);
        ImVec2 valSize = ImGui::CalcTextSize(valBuf);
        dl->AddText(ImVec2(pos.x + availW - valSize.x, pos.y), IM_COL32(60, 165, 255, 255), valBuf);

        // Barra del slider
        float t = std::clamp((*v - mn) / (mx - mn), 0.0f, 1.0f);
        float cy = pos.y + topH + trackAreaH * 0.5f;
        float th = 5.0f;

        ImVec2 trackMin(pos.x, cy - th * 0.5f);
        ImVec2 trackMax(pos.x + availW, cy + th * 0.5f);

        dl->AddRectFilled(trackMin, trackMax, IM_COL32(24, 28, 40, 255), th * 0.5f);
        dl->AddRect(trackMin, trackMax, IM_COL32(55, 65, 88, 140), th * 0.5f, 0, 1.0f);

        float gx = pos.x + kr + t * std::max(1.0f, availW - 2.0f * kr);
        dl->AddRectFilled(trackMin, ImVec2(gx, trackMax.y), IM_COL32(36, 148, 255, 255), th * 0.5f);

        if (held || hovered) {
            dl->AddCircleFilled(ImVec2(gx, cy), kr + 3.0f, IM_COL32(36, 148, 255, 65), 20);
        }
        dl->AddCircleFilled(ImVec2(gx, cy), kr, IM_COL32(36, 148, 255, 255), 20);
        dl->AddCircleFilled(ImVec2(gx, cy), kr - 2.2f, IM_COL32(255, 255, 255, 255), 16);

        return held;
    }

    // Encabezado de tarjeta dentro de un Child Window
    inline void CardHeader(const char* title, const char* subtitle = nullptr, ImU32 accentCol = IM_COL32(36, 148, 255, 255)) {
        ImGuiWindow* w = ImGui::GetCurrentWindow();
        if (w->SkipItems) return;

        ImDrawList* dl = w->DrawList;
        ImVec2 winPos = ImGui::GetWindowPos();
        float winW = ImGui::GetWindowWidth();

        // Línea superior luminosa de la tarjeta
        dl->AddRectFilled(
            winPos,
            ImVec2(winPos.x + winW, winPos.y + 2.0f),
            accentCol,
            10.0f,
            ImDrawFlags_RoundCornersTop
        );

        ImVec2 cur = ImGui::GetCursorScreenPos();
        // Indicador vertical de acento
        dl->AddRectFilled(
            ImVec2(cur.x, cur.y + 2.0f),
            ImVec2(cur.x + 3.5f, cur.y + 18.0f),
            accentCol,
            2.0f
        );

        dl->AddText(ImVec2(cur.x + 11.0f, cur.y + 1.0f), IM_COL32(242, 246, 255, 255), title);

        if (subtitle && subtitle[0] != '\0') {
            ImVec2 titleSz = ImGui::CalcTextSize(title);
            dl->AddText(ImVec2(cur.x + 18.0f + titleSz.x, cur.y + 1.0f), IM_COL32(115, 126, 150, 220), subtitle);
        }

        ImGui::Dummy(ImVec2(0.0f, 22.0f));
        dl->AddLine(
            ImVec2(winPos.x + 12.0f, cur.y + 24.0f),
            ImVec2(winPos.x + winW - 12.0f, cur.y + 24.0f),
            IM_COL32(45, 52, 72, 130),
            1.0f
        );
        ImGui::Dummy(ImVec2(0.0f, 6.0f));
    }

    // Fila de selector de color con etiqueta a la izquierda y botón de color a la derecha
    inline bool ColorRow(const char* label, const char* idStr, ImVec4* color) {
        ImGuiWindow* w = ImGui::GetCurrentWindow();
        if (w->SkipItems) return false;

        float availW = ImGui::GetContentRegionAvail().x;
        ImVec2 pos = ImGui::GetCursorScreenPos();
        float rowH = 24.0f;

        ImVec2 textSz = ImGui::CalcTextSize(label);
        w->DrawList->AddText(
            ImVec2(pos.x, pos.y + (rowH - textSz.y) * 0.5f),
            IM_COL32(178, 188, 208, 255),
            label
        );

        ImGui::SetCursorScreenPos(ImVec2(pos.x + availW - 28.0f, pos.y + 1.0f));
        bool changed = ImGui::ColorEdit4(
            idStr,
            (float*)color,
            ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf
        );
        ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + rowH + 4.0f));
        ImGui::Dummy(ImVec2(availW, 0.0f));
        return changed;
    }

    // Compatibilidad con código previo
    inline void DCM(ImVec2 mn, ImVec2 mx, const char* t, ImU32 acc) {
        ImDrawList* d = ImGui::GetWindowDrawList();
        d->AddRectFilled(mn, mx, IM_COL32(19, 22, 31, 235), 10.0f);
        d->AddRect(mn, mx, IM_COL32(36, 43, 61, 180), 10.0f, 0, 1.0f);
        d->AddRectFilled(mn, ImVec2(mx.x, mn.y + 2.0f), acc, 10.0f, ImDrawFlags_RoundCornersTop);
        d->AddCircleFilled(ImVec2(mn.x + 12.0f, mn.y + 14.0f), 4.0f, acc);
        d->AddText(ImVec2(mn.x + 22.0f, mn.y + 7.0f), IM_COL32(240, 244, 252, 255), t);
    }
}
