#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <string>
#include <iostream>

namespace Bridge {

    inline HWND g_RenderWindow = nullptr;

    inline std::vector<DWORD> GetProcessIdsByName(const std::wstring& processName) {
        std::vector<DWORD> processIds;
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE)
            return processIds;

        PROCESSENTRY32W processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32W);

        if (Process32FirstW(hSnapshot, &processEntry)) {
            do {
                if (_wcsicmp(processName.c_str(), processEntry.szExeFile) == 0) {
                    processIds.push_back(processEntry.th32ProcessID);
                }
            } while (Process32NextW(hSnapshot, &processEntry));
        }

        CloseHandle(hSnapshot);
        return processIds;
    }

    inline BOOL CALLBACK EnumChildWindowsProc(HWND hWnd, LPARAM lParam) {
        char windowName[256] = { 0 };
        GetWindowTextA(hWnd, windowName, sizeof(windowName));
        std::string name(windowName);

        if (name == "_ctl.Window" || name == "HD-Player") {
            g_RenderWindow = hWnd;
            return FALSE;
        }
        return TRUE;
    }

    inline BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam) {
        DWORD processId = *reinterpret_cast<DWORD*>(lParam);
        DWORD windowProcessId;
        GetWindowThreadProcessId(hWnd, &windowProcessId);

        if (windowProcessId == processId && IsWindowVisible(hWnd)) {
            if (!g_RenderWindow) g_RenderWindow = hWnd;
            EnumChildWindows(hWnd, EnumChildWindowsProc, lParam);
        }
        return TRUE;
    }

    inline BOOL CALLBACK GetPosEmlBS(HWND hWnd, LPARAM lParam) {
        char buff[100];
        GetWindowTextA(hWnd, buff, sizeof(buff));
        if (strcmp(buff, "HD-Player") == 0) {
            *reinterpret_cast<HWND*>(lParam) = hWnd;
            return FALSE;
        }
        return TRUE;
    }

    inline HWND FindHDPlayerWindow(HWND parent) {
        HWND target = nullptr;
        EnumChildWindows(parent, GetPosEmlBS, reinterpret_cast<LPARAM>(&target));
        return target;
    }

    inline HWND FindRenderWindow() {
        g_RenderWindow = nullptr;
        auto processIds = GetProcessIdsByName(L"HD-Player.exe");
        if (processIds.empty()) {
            return nullptr;
        }

        for (const auto& procId : processIds) {
            HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
            if (hProc) {
                EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&procId));
                CloseHandle(hProc);
                if (g_RenderWindow) break;
            }
        }
        return g_RenderWindow;
    }

    inline bool GetEmulatorViewport(HWND targetHwnd, int& screenX, int& screenY, int& width, int& height) {
        if (!targetHwnd || !IsWindow(targetHwnd)) return false;

        RECT clientRect;
        POINT screenPos = { 0, 0 };
        if (!GetClientRect(targetHwnd, &clientRect)) return false;
        if (!ClientToScreen(targetHwnd, &screenPos)) return false;

        screenX = screenPos.x;
        screenY = screenPos.y;
        width = clientRect.right - clientRect.left;
        height = clientRect.bottom - clientRect.top;

        return (width > 0 && height > 0);
    }

    inline void KillProcessByName(const wchar_t* processNameW) {
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnap == INVALID_HANDLE_VALUE) return;

        PROCESSENTRY32W pe32{};
        pe32.dwSize = sizeof(pe32);

        if (Process32FirstW(hSnap, &pe32)) {
            do {
                if (_wcsicmp(pe32.szExeFile, processNameW) == 0) {
                    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                    if (hProcess) {
                        TerminateProcess(hProcess, 0);
                        CloseHandle(hProcess);
                    }
                }
            } while (Process32NextW(hSnap, &pe32));
        }
        CloseHandle(hSnap);
    }

    inline void TerminateAllBlueStacksProcesses() {
        const wchar_t* processList[] = {
            L"HD-Player.exe",
            L"HD-Adb.exe",
            L"adb.exe",
            L"BstkSVC.exe",
            L"BlueStacks.exe",
            L"BlueStacksAppPlayer.exe",
            L"HD-Agent.exe",
            L"HD-Service.exe",
            L"BstkAgent.exe",
            nullptr
        };

        for (int i = 0; processList[i] != nullptr; i++) {
            KillProcessByName(processList[i]);
        }
    }

} // namespace Bridge
