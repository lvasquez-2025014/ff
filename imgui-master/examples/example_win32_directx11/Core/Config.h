#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <imgui.h>
#include <string>

namespace Core {

    struct ConfigData {
        // Visuals (ESP)
        bool EspLineEnabled = true;
        bool EspBoxEnabled = true;
        int  EspBoxStyle = 1; // 0: 2D Box, 1: Corner Box, 2: Filled Box
        bool EspHealthEnabled = true;
        bool EspSkeletonEnabled = true;
        bool EspNameEnabled = true;
        bool ShowTeam = false;
        float LineThickness = 1.5f;
        float BoxThickness = 1.2f;
        float SkeletonThickness = 1.3f;
        ImVec4 ColorEnemy = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImVec4 ColorTeam = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
        ImVec4 ColorBox = ImVec4(0.11f, 0.55f, 0.98f, 1.0f);
        ImVec4 ColorSkeleton = ImVec4(0.88f, 0.92f, 1.0f, 0.90f);
        ImVec4 ColorName = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

        // Combat & Exploits (Silent Aim)
        bool SilentAimEnabled = false;
        float SilentFov = 120.0f;
        int SilentBone = 0; // 0: Cabeza, 1: Cuello, 2: Pecho
        int SilentMode = 0; // 0: FOV Normal, 1: 360 Grados
        float SilentPower = 1.0f;
        bool DrawSilentFov = true;
        ImVec4 ColorSilentFov = ImVec4(0.11f, 0.55f, 0.98f, 0.85f);

        // UI & Aesthetics
        bool MenuOpen = true;
        bool IsClickable = true;
        bool Verified = true;
        int ActiveTab = 0;
        float TabAlpha = 1.0f;
        ImVec4 ThemeAccent = ImVec4(0.0f, 0.65f, 1.0f, 1.0f);
        // --- AIMBOT VISIBLE ---
        bool AimbotEnabled = false;
        bool AimbotDrawFov = true;
        float AimbotFov = 150.0f;      // Radio del círculo
        float AimbotSmooth = 3.0f;     // 1.0 = Muy rápido, 10.0 = Muy suave
        int AimbotKey = 0x02;          // 0x02 = Click Derecho, 0x05 = Click Izquierdo

        // Authentication & Session
        bool Authenticated = false;
        bool ShowLogin = true;
        bool Loading = false;
        float Progress = 0.0f;
        char Username[255] = "";
        char Password[255] = "";
        // Silent Aim - tasas Head/Chest (de BrutalSilent)
        int  SilentHeadRate = 30;      // % de disparos a la cabeza
        int  SilentChestRate = 70;      // % de disparos al pecho
        bool SilentFullHeadshot = true;    // ★ true = siempre cabeza (recomendado)
        bool SilentIgnoreKnocked = false;   // ignorar enemigos noqueados
        bool SilentNoRecoil = true;        // ★ No Recoil mientras el silent este activo
        // Bridge & ADB Connection
        bool AdbConnected = false;
        bool AdbConnecting = false;
        std::string AdbStatus = "ADB STANDBY";
        HWND EmulatorHwnd = nullptr;
        HWND RenderWindow = nullptr;
    };

    inline ConfigData& GetConfig() {
        static ConfigData instance;
        return instance;
    }

} // namespace Core
