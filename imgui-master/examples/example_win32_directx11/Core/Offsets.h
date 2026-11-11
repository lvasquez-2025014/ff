#pragma once
#include <cstdint>
#include <string>

namespace Core {
    namespace Offsets {
        // --- BASE ---
        inline std::string GamePackage = "com.dts.freefireth";
        inline uintptr_t Il2Cpp = 0x0;
        inline uintptr_t Libunity = 0x0;
        inline uintptr_t InitBase = 0xA342EFC;
        inline uintptr_t StaticClass = 0x5C;

        // --- MATCH ---
        inline uintptr_t CurrentMatch = 0x50;
        inline uintptr_t MatchStatus = 0x8C;
        inline uintptr_t LocalPlayer = 0x94;
        inline uintptr_t DictionaryEntities = 0x68;

        // --- PLAYER ---
        inline uintptr_t Player_IsDead = 0x50;
        inline uintptr_t Player_Name = 0x31C;
        inline uintptr_t OriginalName = 0x320;
        inline uintptr_t Player_Data = 0x48;
        inline uintptr_t Player_IsBot = 0x324;
        inline uintptr_t LocalPlayerAttributes = 0x500;
        inline uintptr_t Speed = 0x3BC;
        inline uintptr_t isFiringOffset = 0x58C;
        inline uintptr_t Player_ShadowBase = 0x1A60;
        inline uintptr_t XPose = 0x78;

        // --- CAMERA ---
        inline uintptr_t AimRotation = 0x440;
        inline uintptr_t FollowCamera = 0x494;
        inline uintptr_t Camera = 0x18;
        inline uintptr_t MainCameraTransform = 0x28C;
        inline uintptr_t ViewMatrix = 0xE8;

        // --- AVATAR ---
        inline uintptr_t AvatarManager = 0x504;
        inline uintptr_t Avatar = 0xA8;
        inline uintptr_t Avatar_IsVisible = 0x95;
        inline uintptr_t Avatar_Data = 0x14;
        inline uintptr_t Avatar_Data_IsTeam = 0x59;

        // --- AIM / SILENT AIM ---
        inline uintptr_t sAim1 = 0x58C;
        inline uintptr_t sAim2 = 0x9D0;
        inline uintptr_t sAim3 = 0x38;
        inline uintptr_t sAim4 = 0x2C;

        // Compatibilidad (tu silent.h usa estos nombres)
        inline uintptr_t AimSilent1 = 0x9D0; // WeaponInstance
        inline uintptr_t AimSilent2 = 0x38;  // RayDir
        inline uintptr_t AimSilent3 = 0x2C;  // StartPos

        // --- WEAPON ---
        inline uintptr_t Weapon = 0x434;
        inline uintptr_t WeaponData = 0x58;
        inline uintptr_t WeaponRecoil = 0x0C;
        inline uintptr_t AmmoOffs = 0x6A4;

        // --- WEAPON GUN FEEL (NUEVO - Para el No Recoil) ---
        inline uintptr_t Weapon_GunFeelProfile = 0x3B0;
        inline uintptr_t WGP_EnableRecoil = 0x1D0;
        inline uintptr_t WGP_EnableSpread = 0x1D6;
        inline uintptr_t WGP_EnableGunFeel = 0x1D4;

        // --- BONES ---
        inline uintptr_t Head = 0x49C;
        inline uintptr_t Spine = 0x4A4;
        inline uintptr_t Neck = 0x4A4;
        inline uintptr_t Hip = 0x4A0;
        inline uintptr_t Root = 0x4B0;
        inline uintptr_t LeftSholder = 0x4D0;
        inline uintptr_t RightSholder = 0x4D4;
        inline uintptr_t LeftElbow = 0x4E4;
        inline uintptr_t RightElbow = 0x4E0;
        inline uintptr_t LeftWrist = 0x4DC;
        inline uintptr_t RightWrist = 0x4D8;
        inline uintptr_t LeftHand = 0x4C8;
        inline uintptr_t RightHand = 0x498;
        inline uintptr_t LeftCalf = 0x4B8;
        inline uintptr_t RightCalf = 0x4BC;
        inline uintptr_t LeftFoot = 0x4C0;
        inline uintptr_t RightFoot = 0x4C4;

        // --- INICIALIZACIÓN ---
        inline void Initialize(int game = 0) {
            // Los offsets ya están inicializados arriba por defecto para FF Normal.
            // Si tuvieras FF MAX o India, podrías poner un if(game == 1) aquí.
        }
    }
}
