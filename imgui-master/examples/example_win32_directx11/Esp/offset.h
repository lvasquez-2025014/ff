#pragma once
#include <cstdint>
#include <string>
// Offsets Principales

std::string GamePackage;
uintptr_t Il2Cpp;
uintptr_t Libunity;
uintptr_t WallHackLocation;
uintptr_t NightLocation;
uintptr_t InitBase;
uintptr_t StaticClass;
uintptr_t DictionaryEntities;
uintptr_t WukongOrion;
uintptr_t ViewMatrix;


uintptr_t MatchStatus;
uintptr_t CurrentMatch;


uintptr_t LocalPlayer;
uintptr_t Player_IsDead;
uintptr_t Player_Name;
uintptr_t Player_Data;
uintptr_t Player_ShadowBase;
uintptr_t AvatarManager;
uintptr_t Avatar;
uintptr_t Avatar_IsVisible;
uintptr_t Avatar_Data;
uintptr_t Avatar_Data_IsTeam;
uintptr_t HealdShieldEP;
uintptr_t XPose;


uintptr_t FollowCamera;
uintptr_t Camera;
uintptr_t AimRotation;
uintptr_t MainCameraTransform;


uintptr_t Weapon;
uintptr_t WeaponData;
uintptr_t WeaponRecoil;

uintptr_t LocalPlayerAttributes;
uintptr_t NoReloadOffset;
uintptr_t MedikitOffset;
uintptr_t Speed;
uintptr_t RageCollider;
uintptr_t isFiringOffset;
uintptr_t BaseProfileInfo;
uintptr_t PlayerIDOffset;
uintptr_t AmmoOffs;
uintptr_t FasFireOffs;
uintptr_t OffsQuickSwitch;
uintptr_t AimSilent1;
uintptr_t AimSilent2;
uintptr_t AimSilent3;


uintptr_t Head;


uintptr_t Spine;
uintptr_t Root;
uintptr_t Hip;


uintptr_t LeftSholder;
uintptr_t LeftElbow;
uintptr_t LeftWrist;
uintptr_t LeftHand;


uintptr_t RightSholder;
uintptr_t RightElbow;
uintptr_t RightWrist;
uintptr_t RightHand;


uintptr_t LeftCalf;
uintptr_t LeftFoot;


uintptr_t RightCalf;
uintptr_t RightFoot;

// Inicializar offsets según el juego
void initOffsets(int game)
{
    if (game == 0) {
        // FF Normal
        GamePackage = "com.dts.freefireth";

        Il2Cpp = 0x0;
        Libunity = 0x0;
        InitBase = 0xA342EFC;
        StaticClass = 0x5C;

        WukongOrion = 0xB50;
        WallHackLocation = 0x152570E;
        NightLocation = 0x2F89FF;

        CurrentMatch = 0x50;
        MatchStatus = 0x8c;

        LocalPlayer = 0x94;
        DictionaryEntities = 0x68;

        Player_IsDead = 0x50;
        Player_Name = 0x31C ;
        Player_Data = 0x48;
        RageCollider = 0x4E8;

        Player_ShadowBase = 0x1A60;
        XPose = 0x78;

        AvatarManager = 0x504;
        Avatar = 0xA8;
        Avatar_IsVisible = 0x95;
        Avatar_Data = 0x14;
        Avatar_Data_IsTeam = 0x59;

        FollowCamera = 0x494;
        Camera = 0x18;
        AimRotation = 0x440;
        MainCameraTransform = 0x28C;

        Weapon = 0x434;
        WeaponData = 0x58;
        WeaponRecoil = 0x0C;

        ViewMatrix = 0xE8;
        LocalPlayerAttributes = 0x500;
        isFiringOffset = 0x58C;
        BaseProfileInfo = 0x18CC;
        HealdShieldEP = 0x10;
        NoReloadOffset = 0xC1;
        AimSilent1 = 0x9D0;
        AimSilent2 = 0x38;
        AimSilent3 = 0x2C;

        // Offsets de Huesos (Última actualización)
        Head = 0x49C;            // Cabeza

        // === Torso / Core ===
        Spine = 0x4A4;           // Columna (Ahora unificado con Neck en el offset nuevo)
        Root = 0x4B0;            // Raíz
        Hip = 0x4A0;             // Cadera

        // === Brazo Izquierdo ===
        LeftSholder = 0x4D0;     // Hombro Izquierdo
        LeftElbow = 0x4E4;       // Codo Izquierdo
        LeftWrist = 0x4DC;       // Muñeca Izquierda
        LeftHand = 0x4C8;        // Mano Izquierda

        // === Brazo Derecho ===
        RightSholder = 0x4D4;    // Hombro Derecho
        RightElbow = 0x4E0;      // Codo Derecho
        RightWrist = 0x4D8;      // Muñeca Derecha
        RightHand = 0x498;       // Mano Derecha

        // === Pierna Izquierda ===
        LeftCalf = 0x4B8;        // Pantorrilla / Tobillo Izquierdo (Usando LeftAnkle)
        LeftFoot = 0x4C0;        // Pie Izquierdo

        // === Pierna Derecha ===
        RightCalf = 0x4BC;       // Pantorrilla / Tobillo Derecho (Usando RightAnkle)
        RightFoot = 0x4C4;       // Pie Derecho
    }

}
