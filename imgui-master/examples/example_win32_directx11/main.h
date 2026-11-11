#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#define IMGUI_DEFINE_MATH_OPERATORS


#include <windows.h>
#include <ntstatus.h>         
#include <winternl.h>
#include <TlHelp32.h>


#include "Auth/Auth.hpp"
#include "Auth/xorstr.hpp"
#include "Esp/Esp_line.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include "imgui_settings.h"
#include <d3d11.h>
#include <D3DX11tex.h>
#include <dwmapi.h>
#include <tchar.h>

#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <thread>
#include "image.h"
#include "Font.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DX11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ntdll.lib")

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HANDLE hCurrentUIThread = nullptr;
HANDLE hThread1 = nullptr;
HANDLE hThread2 = nullptr;

HWND hwnd;
RECT rc;

int UpCheckx;
bool aimsilent = false;
int headRate = 3;
int chestRate = 3;
bool adbboton = false;
bool isClickable = true;
bool Main_Window = true;
bool Verified = true;
bool login = true;
bool registrard = false;
bool open = true;

bool authenticed = false;
bool loading = false;
bool show_login = true;
static float load_speed = 0.25f; // Velocidad de carga, ajusta como desees (0.25 = ~4s)
static float progress1 = 0.0f;


int page = 0;



static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;
ImFont* Inter_S = nullptr;
ImFont* Inter_S_1 = nullptr;
ImFont* Inter_S_2 = nullptr;
ImFont* Inter_S_3 = nullptr;
ImFont* Inter_B = nullptr;
ImFont* Icon = nullptr;
ImFont* Icon_Arrow;
static float anim_text = 0.f;

namespace font1
{
    inline ImFont* global_default1 = nullptr;
    inline ImFont* lexend_tabs1 = nullptr;
    inline ImFont* icon_tabs1 = nullptr;
    inline ImFont* icon_big1 = nullptr;
    inline ImFont* name_menu1 = nullptr;
}
namespace texture
{
    inline ID3D11ShaderResourceView* bg = nullptr;
    inline ID3D11ShaderResourceView* bg_menu = nullptr;
    inline ID3D11ShaderResourceView* bg_blur = nullptr;
}
namespace image
{

    ID3D11ShaderResourceView* settings_checkbox = nullptr;
    ID3D11ShaderResourceView* user = nullptr;
    ID3D11ShaderResourceView* fps = nullptr;
    ID3D11ShaderResourceView* ms = nullptr;

    ID3D11ShaderResourceView* logo = nullptr;
}
D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
ID3D11ShaderResourceView* logo = nullptr;
inline ImVec2 size_login = ImVec2(455, 380);

ID3D11ShaderResourceView* fu1l = nullptr;

static float tab_alpha_2 = 0.f; /* */ static float tab_add_2; /* */ static int active_tab_2 = 0;
static float tab_alpha_1 = 0.f; /* */ static float tab_add_1; /* */ static int active_tab_1 = 0;

static int page_switch = 0;

template<typename T> static inline T ImLerp1(T a, T b, float t) { return (T)(a + (b - a) * t); }
int SkinsPage = 0;

namespace image2
{
    ID3D11ShaderResourceView* rayo = nullptr;
    ID3D11ShaderResourceView* flecha = nullptr;
    ID3D11ShaderResourceView* ancla = nullptr;
    ID3D11ShaderResourceView* fastfire = nullptr;
    ID3D11ShaderResourceView* tpenemy = nullptr;
    ID3D11ShaderResourceView* nieve = nullptr;
    ID3D11ShaderResourceView* ghost = nullptr;
    ID3D11ShaderResourceView* ffimagenMAX = nullptr;
    ID3D11ShaderResourceView* ffimagenTELA = nullptr;
    ID3D11ShaderResourceView* ffimagen = nullptr;

    ID3D11ShaderResourceView* main = nullptr;
    ID3D11ShaderResourceView* gless = nullptr;
    ID3D11ShaderResourceView* help = nullptr;
    ID3D11ShaderResourceView* settings = nullptr;
    ID3D11ShaderResourceView* panel = nullptr;
    ID3D11ShaderResourceView* other = nullptr;
    ID3D11ShaderResourceView* save = nullptr;
    ID3D11ShaderResourceView* favourites = nullptr;
    ID3D11ShaderResourceView* loginiconoxcy = nullptr;
    ID3D11ShaderResourceView* login111 = nullptr;
    ID3D11ShaderResourceView* Register111 = nullptr;
}

namespace font2 {
    inline ImFont* fuentezada = nullptr;


}


// ===============================
// SHADOW RECT
// ===============================
inline void AddShadowRect(
    ImDrawList* draw,
    ImVec2 min,
    ImVec2 max,
    ImColor color,
    float thickness,
    ImVec2 offset = ImVec2(0, 0),
    float rounding = 0.0f,
    int layers = 20
)
{
    for (int i = 0; i < layers; i++)
    {
        float t = (float)i / layers;
        float alpha = color.Value.w * (1.0f - t);

        ImColor col = ImColor(color.Value.x, color.Value.y, color.Value.z, alpha * 0.6f);
        float grow = t * thickness;

        draw->AddRectFilled(
            ImVec2(min.x - grow + offset.x, min.y - grow + offset.y),
            ImVec2(max.x + grow + offset.x, max.y + grow + offset.y),
            col,
            rounding + grow
        );
    }
}

// ===============================
// SHADOW CIRCLE
// ===============================
inline void AddShadowCircle(
    ImDrawList* draw,
    ImVec2 center,
    float radius,
    ImColor color,
    float thickness,
    ImVec2 offset = ImVec2(0, 0),
    int layers = 20
)
{
    for (int i = 0; i < layers; i++)
    {
        float t = (float)i / layers;
        float alpha = color.Value.w * (1.0f - t);

        ImColor col = ImColor(color.Value.x, color.Value.y, color.Value.z, alpha * 0.6f);
        float grow = t * thickness;

        draw->AddCircleFilled(
            ImVec2(center.x + offset.x, center.y + offset.y),
            radius + grow,
            col,
            64
        );
    }
}

// ===============================
// SHADOW TEXT
// ===============================
inline void ShadowText(
    ImDrawList* draw,
    const char* text,
    ImVec2 pos,
    ImColor text_color,
    ImColor shadow_color,
    float size = 20.0f,
    int layers = 10
)
{
    for (int i = 0; i < layers; i++)
    {
        float t = (float)i / layers;
        float alpha = shadow_color.Value.w * (1.0f - t);

        ImColor col = ImColor(
            shadow_color.Value.x,
            shadow_color.Value.y,
            shadow_color.Value.z,
            alpha * 0.5f
        );

        float offset = t * size * 0.05f;

        draw->AddText(
            ImVec2(pos.x + offset, pos.y + offset),
            col,
            text
        );
    }

    draw->AddText(pos, text_color, text);
}

