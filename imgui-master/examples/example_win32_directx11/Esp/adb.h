#include <string>
extern std::string EspADB;
#pragma once
#include "vector.h"
#include "lib.h"
#include "..\MinHook\include\MinHook.h"
#include "offset.h"
DWORD GetProcZ(const char* processName)
{
    DWORD processID = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe32))
        {
            do
            {
                if (strcmp(pe32.szExeFile, processName) == 0)
                {
                    processID = pe32.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
    }
    return processID;
}

bool CheckProcessInstancesZ(const char* processName, int& count) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (pe32.szExeFile == processName) {
                count++;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return true;
}

bool KillProcZ(DWORD processID)
{
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
    if (hProcess == NULL)
    {
        return false;
    }
    bool result = TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);
    return result;
}

bool IsProcRunZ(const char* processName, DWORD& processID)
{
    bool isRunning = false;
    processID = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return false;
    }
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (strcmp(pe32.szExeFile, processName) == 0) {
                isRunning = true;
                processID = pe32.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);

    return isRunning;
}

bool ForgeKillProcZ(DWORD processID)
{
    HWND hwnd = NULL;
    DWORD dwPID = 0;
    do {
        hwnd = FindWindowEx(NULL, hwnd, NULL, NULL);
        GetWindowThreadProcessId(hwnd, &dwPID);
    } while (dwPID != processID && hwnd != NULL);

    if (hwnd != NULL)
    {
        PostMessage(hwnd, WM_CLOSE, 0, 0);
        return true;
    }
    return false;
}

static auto lastCleanTime = std::chrono::steady_clock::now();
std::mutex cacheMutex;  // ProtecciÃ³n para acceso concurrente al cachÃ©
std::atomic<bool> runningxd2(true);

// Estructura de cachÃ© optimizada
std::unordered_map<uintptr_t, uintptr_t> CacheZ;
std::shared_mutex zCacheSharMtx;
std::unordered_map<uint32_t, Vector3> enemiesSunk;
std::unordered_set<uint32_t> enemiesAlreadySunk;
const auto zTimeMs = std::chrono::milliseconds(300);

enum class AimKey {
    RightMouseButton,
    LeftMouseButton,
    MiddleMouseButton,
    ShiftKey,
    ControlKey,
    AltKey
};

AimKey selectedKey = AimKey::LeftMouseButton;
MatchStatusEnum currentMatchStatus = MATCH_NOT_STARTED;

struct BonePositions
{
    // Parte superior
    Vector3 Cabeza, Columna, Raiz, Cadera;

    // Brazos
    Vector3 HombroIzquierdo, HombroDerecho;
    Vector3 CodoIzquierdo, CodoDerecho;
    Vector3 MunecaIzquierda, MunecaDerecha;
    Vector3 ManoIzquierda, ManoDerecha;

    // Piernas
    Vector3 PantorrillaIzquierda, PantorrillaDerecha;
    Vector3 PieIzquierdo, PieDerecho;
};

BonePositions bonePositions;

// Variables globales optimizadas
void* vmPtr = nullptr;
void* pVMAddr = nullptr;
void* cpuAddr = nullptr;
int zCacheSize = 5000;  // Reducido para gama baja

// Tipos de funciones optimizadas
typedef int(__cdecl* PGMPhysReadFunc)(void*, uintptr_t, void*, size_t);
typedef int(__cdecl* PGMPhysSimpleWriteGCPhysFunc)(void*, uintptr_t, void*, size_t);
typedef int(__cdecl* PGMPhysGCPtr2GCPhysFunc)(void*, uintptr_t, uintptr_t*);
typedef void* (__cdecl* VMMGetCpuByIdFunc)(void*, int);

// Funciones originales
PGMPhysReadFunc ogPhysRead = nullptr;
VMMGetCpuByIdFunc ogCPU = nullptr;
PGMPhysGCPtr2GCPhysFunc ogCast = nullptr;
PGMPhysSimpleWriteGCPhysFunc ogWrite = nullptr;

// FunciÃ³n de escritura optimizada
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
    size_t total = size * nmemb;
    buffer->append((char*)contents, total);
    return total;
}

// Hooks optimizados
int __cdecl HookedPGMPhysRead(void* pVM, uintptr_t GCPhys, void* pvBuf, size_t cbRead) {
    if (!vmPtr) vmPtr = pVM;
    return ogPhysRead(pVM, GCPhys, pvBuf, cbRead);
}

int zHookWrite(void* pVM, uintptr_t GCPhys, void* pvBuf, size_t cbRead) {
    return ogWrite(pVM, GCPhys, pvBuf, cbRead);
}

int zHookRead(void* pVM, uintptr_t GCPhys, void* pvBuf, size_t cbRead) {
    return ogPhysRead(pVM, GCPhys, pvBuf, cbRead);
}

void* CPU(void* pVM, int cpuId) {
    return ogCPU(pVM, cpuId);
}

int Cast(void* pVCpu, uintptr_t address, uintptr_t* physAddress) {
    return ogCast(pVCpu, address, physAddress);
}

void InitializeZ(void* pVM)
{
    pVMAddr = pVM;
    if (pVM && ogCPU)
        cpuAddr = CPU(pVM, 0);
    CacheZ.clear();
}

// Reset ESP - Seguro y verificado
bool ConvertZ(uintptr_t address, uintptr_t& phys) {
    auto it = CacheZ.find(address);
    if (it != CacheZ.end()) {
        phys = it->second;
        return true;
    }

    if (!pVMAddr || !ogCPU || !ogCast) return false;
    if (!cpuAddr && !(cpuAddr = CPU(pVMAddr, 0))) return false;

    int status = Cast(cpuAddr, address, &phys);
    if (status != 0) return false;

    CacheZ[address] = phys;
    return true;
}

// Lectura optimizada
template<typename T>
bool ReadZ(uintptr_t address, T& data) {
    uintptr_t physAddress;
    return ConvertZ(address, physAddress) && (zHookRead(pVMAddr, physAddress, &data, sizeof(T)) == 0);
}

// Lectura de arrays optimizada
template<typename T>
bool ReadArrayZ(uintptr_t address, std::vector<T>& array) {
    uintptr_t physAddress;
    if (!ConvertZ(address, physAddress)) return false;

    return zHookRead(pVMAddr, physAddress, array.data(), sizeof(T) * array.size()) == 0;
}

template<typename T>
bool ReadArrayZ2(uintptr_t address, std::vector<T>& array)
{
    uintptr_t convertedAddress;
    bool result = ConvertZ(address, convertedAddress);

    if (!result)
        return false;

    size_t size = sizeof(T) * array.size();
    DWORD status = zHookRead(pVMAddr, convertedAddress, array.data(), size);

    return status == 0;
}

// ConversiÃ³n de wstring a string optimizada
std::string WideStringToString(const std::wstring& wideString) {
    if (wideString.empty()) {
        return std::string();
    }

    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), (int)wideString.length(), NULL, 0, NULL, NULL);

    std::string result(sizeNeeded, 0);


    WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), (int)wideString.length(), &result[0], sizeNeeded, NULL, NULL);

    return result;
}

// Lectura de strings optimizada
std::string ReadStringZ2(uintptr_t address, int size, bool unicode = true)
{
    std::vector<uint8_t> stringBytes(size);

    bool read = ReadArrayZ2(address, stringBytes);

    if (!read) return "";

    std::string readString;
    if (unicode) {

        std::wstring wideString(reinterpret_cast<wchar_t*>(stringBytes.data()), size / 2);
        readString = WideStringToString(wideString);
    }
    else {

        readString = std::string(stringBytes.begin(), stringBytes.end());
    }

    auto nullTerminator = readString.find('\0');
    if (nullTerminator != std::string::npos)
        readString = readString.substr(0, nullTerminator);

    return readString;
}

// Escritura optimizada
template<typename T>
void WriteZ(uintptr_t address, const T& value) {
    uintptr_t physAddress;
    if (ConvertZ(address, physAddress)) {
        zHookWrite(pVMAddr, physAddress, (void*)&value, sizeof(T));
    }
}

// Estructuras de datos optimizadas
struct TMatrix {
    Vector4 position;
    Quaternion rotation;
    Vector4 scale;
};

struct ShieldData {
    float escudoPorcentaje;
    std::string escudoStr;
};

struct HealthData {
    float vidaPorcentaje;
    std::string vidaStr;
};

struct BoxData {
    ImVec2 topLeft;
    ImVec2 bottomRight;
};
// FunciÃ³n GetPosition optimizada
bool GetPosition(uint32_t transform, Vector3& pos)
{
    pos = Vector3::Zero();
    uint32_t transformObjValue;
    if (!ReadZ(transform + 0x8, transformObjValue)) return false;

    uint32_t indexValue;
    if (!ReadZ(transformObjValue + 0x24, indexValue)) return false;

    uint32_t matrixValue;
    if (!ReadZ(transformObjValue + 0x20, matrixValue)) return false;

    uint32_t matrixListValue;
    if (!ReadZ(matrixValue + 0x18, matrixListValue)) return false;

    uint32_t matrixIndicesValue;
    if (!ReadZ(matrixValue + 0x1C, matrixIndicesValue)) return false;

    Vector3 resultValue;
    if (!ReadZ(indexValue * 0x30 + matrixListValue, resultValue)) return false;

    int maxTries = 50;
    int tries = 0;
    int transformIndexValue;
    if (!ReadZ((uint32_t)((indexValue * 0x4) + matrixIndicesValue), transformIndexValue)) return false;

    while (transformIndexValue >= 0)
    {
        tries++;
        if (tries == maxTries) break;
        TMatrix tMatrixValue;
        if (!ReadZ((uint32_t)(0x30 * transformIndexValue + matrixListValue), tMatrixValue)) return false;

        float rotX = tMatrixValue.rotation.x;
        float rotY = tMatrixValue.rotation.y;
        float rotZ = tMatrixValue.rotation.z;
        float rotW = tMatrixValue.rotation.w;
        float scaleX = resultValue.x * tMatrixValue.scale.x;
        float scaleY = resultValue.y * tMatrixValue.scale.y; // â CORREGIDO
        float scaleZ = resultValue.z * tMatrixValue.scale.z;

        resultValue.x = (float)(tMatrixValue.position.x + scaleX +
            (scaleX * ((rotY * rotY * -2.0) - (rotZ * rotZ * 2.0))) +
            (scaleY * ((rotW * rotZ * -2.0) - (rotY * rotX * -2.0))) +
            (scaleZ * ((rotZ * rotX * 2.0) - (rotW * rotY * -2.0))));
        resultValue.y = (float)(tMatrixValue.position.y + scaleY +
            (scaleX * ((rotX * rotY * 2.0) - (rotW * rotZ * -2.0))) +
            (scaleY * ((rotZ * rotZ * -2.0) - (rotX * rotX * 2.0))) +
            (scaleZ * ((rotW * rotX * -2.0) - (rotZ * rotY * -2.0))));
        resultValue.z = (float)(tMatrixValue.position.z + scaleZ +
            (scaleX * ((rotW * rotY * -2.0) - (rotX * rotZ * -2.0))) +
            (scaleY * ((rotY * rotZ * 2.0) - (rotW * rotX * -2.0))) +
            (scaleZ * ((rotX * rotX * -2.0) - (rotY * rotY * 2.0))));

        if (!ReadZ((uint32_t)(transformIndexValue * 0x4 + matrixIndicesValue), transformIndexValue)) return false;
    }
    pos = resultValue;
    return tries != maxTries;
}

bool GetNodePosition(uint32_t nodeTransform, Vector3& result) {
    uint32_t transformValue;
    return ReadZ(nodeTransform + 0x8, transformValue) && GetPosition(transformValue, result);
}

// FunciÃ³n para encontrar ventana HD-Player optimizada
BOOL CALLBACK GetPosEmlBS(HWND hWnd, LPARAM lParam) {
    char title[256];
    GetWindowTextA(hWnd, title, sizeof(title));
    std::string windowName(title);

    if (windowName == "HD-Player" || windowName == "_ctl.Window") {
        *reinterpret_cast<HWND*>(lParam) = hWnd;
        return FALSE;
    }
    return TRUE;
}

HWND FindHDPlayerWindow(HWND parent) {
    HWND renderWindow = nullptr;
    EnumChildWindows(parent, GetPosEmlBS, reinterpret_cast<LPARAM>(&renderWindow));
    return renderWindow;
}

std::string ErrorLoadAdb = "NUll";

std::string GetExeDirectoryZ() {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    std::string fullPath(path);
    size_t lastSlashIndex = fullPath.find_last_of("\\/");
    if (lastSlashIndex != std::string::npos) {
        return fullPath.substr(0, lastSlashIndex);
    }
    return "";
}

std::string GetExeDirectoryPIDZ(DWORD processID) {
    char path[MAX_PATH];
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

    if (hProcess) {
        if (GetModuleFileNameExA(hProcess, NULL, path, MAX_PATH)) {
            std::string fullPath(path);
            size_t lastSlashIndex = fullPath.find_last_of("\\/");
            CloseHandle(hProcess);
            if (lastSlashIndex != std::string::npos) {
                return fullPath.substr(0, lastSlashIndex);
            }
        }
        CloseHandle(hProcess);
    }

    return "";
}

bool CambiarDirectoryZ(const std::string& path) {
    return SetCurrentDirectoryA(path.c_str());
}

void KillAdbZ() {
    DWORD ProcIdAdb = GetProcZ("adb.exe");
    DWORD ProcIdAdb2 = 0;
    KillProcZ(ProcIdAdb);
    if (IsProcRunZ("adb.exe", ProcIdAdb2)) {
        ForgeKillProcZ(ProcIdAdb2);
    }
    DWORD ProcIdAdbHD = GetProcZ("HD-Adb.exe");
    DWORD ProcIdAdbHD2 = 0;
    KillProcZ(ProcIdAdbHD);
    if (IsProcRunZ("HD-Adb.exe", ProcIdAdbHD2)) {
        ForgeKillProcZ(ProcIdAdbHD2);
    }

    // Esperar reducida para no causar delays innecesarios
    Sleep(50);
}

std::string ExtraerLibAddressZ(const std::string& input) {
    std::size_t pos = input.find('-');
    if (pos != std::string::npos && pos >= 8) {
        return input.substr(pos - 8, 8);
    }
    return "";
}

inline uintptr_t uIntExtrZ(std::string c, int base = 16) {
    static_assert(sizeof(uintptr_t) == sizeof(unsigned long)
        || sizeof(uintptr_t) == sizeof(unsigned long long),
        "Please add string to handle conversion for this architecture.");
    if (sizeof(uintptr_t) == sizeof(unsigned long)) {
        return strtoul(c.c_str(), nullptr, base);
    }
    return strtoull(c.c_str(), nullptr, base);
}

// FunciÃ³n duplicada para libunity.so
inline uintptr_t uIntExtrZ2(std::string c, int base = 16) {
    static_assert(sizeof(uintptr_t) == sizeof(unsigned long)
        || sizeof(uintptr_t) == sizeof(unsigned long long),
        "Please add string to handle conversion for this architecture.");
    if (sizeof(uintptr_t) == sizeof(unsigned long)) {
        return strtoul(c.c_str(), nullptr, base);
    }
    return strtoull(c.c_str(), nullptr, base);
}

static std::string AdbRunCapture_NoWindow(const std::string& args)
{
    std::string cmdLine = ".\\HD-Adb " + args;

    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;

    HANDLE hStdOutRead = NULL, hStdOutWrite = NULL;
    if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) return "";
    SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0);

    HANDLE hStdInRead = NULL, hStdInWrite = NULL;
    if (!CreatePipe(&hStdInRead, &hStdInWrite, &sa, 0)) {
        CloseHandle(hStdOutRead); CloseHandle(hStdOutWrite);
        return "";
    }
    SetHandleInformation(hStdInWrite, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si{};
    si.cb = sizeof(si);
    si.hStdOutput = hStdOutWrite;
    si.hStdError = hStdOutWrite;
    si.hStdInput = hStdInRead;
    si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi{};
    if (!CreateProcessA(
        NULL,
        const_cast<LPSTR>(cmdLine.c_str()),
        NULL, NULL,
        TRUE,                 // heredar handles
        CREATE_NO_WINDOW,     // sin consola
        NULL, NULL,
        &si, &pi))
    {
        CloseHandle(hStdOutRead); CloseHandle(hStdOutWrite);
        CloseHandle(hStdInRead);  CloseHandle(hStdInWrite);
        return "";
    }

    // Cierro extremos que no uso
    CloseHandle(hStdInRead);
    CloseHandle(hStdInWrite);
    CloseHandle(hStdOutWrite);

    // Leo salida
    std::string output;
    CHAR buffer[4096];
    DWORD read = 0;
    while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &read, NULL) && read > 0) {
        buffer[read] = '\0';
        output += buffer;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(hStdOutRead);

    return output;
}

static std::vector<std::string> ParseAdbDevices_List(const std::string& out)
{
    std::vector<std::string> devices;
    std::istringstream iss(out);
    std::string line;
    while (std::getline(iss, line)) {
        if (line.find("List of devices") != std::string::npos) continue;
        // Formato: "<deviceId>\tdevice"
        auto tab = line.find('\t');
        if (tab != std::string::npos) {
            auto id = line.substr(0, tab);
            if (!id.empty() && line.find("device") != std::string::npos)
                devices.push_back(id);
        }
    }
    return devices;
}

static std::string GetFirstAdbDeviceId()
{
    // 1) Intento directo
    {
        const auto out = AdbRunCapture_NoWindow("devices");
        auto list = ParseAdbDevices_List(out);
        if (!list.empty())
            return list[0];
    }

    // 2) Conectar a puertos comunes sin mostrar CMD
    static const int ports[] = { 5555, 5556, 5557, 5558, 5559, 5510 };
    for (int port : ports) {
        AdbRunCapture_NoWindow(std::string("connect 127.0.0.1:") + std::to_string(port));
        Sleep(50);  // Restaurado al valor original

        // Verificar si la conexiÃ³n fue exitosa
        const auto checkOut = AdbRunCapture_NoWindow("devices");
        auto checkList = ParseAdbDevices_List(checkOut);
        if (!checkList.empty()) {
            break;  // ConexiÃ³n exitosa, no intentar mÃ¡s puertos
        }
    }

    // 3) Reintento
    {
        const auto out2 = AdbRunCapture_NoWindow("devices");
        auto list2 = ParseAdbDevices_List(out2);
        return list2.empty() ? std::string() : list2[0];
    }
}

std::string ShellGetAddressZ(const std::string& firstCommand, const std::string& secondCommand) {
    HANDLE hStdOutRead, hStdOutWrite;
    HANDLE hStdInRead, hStdInWrite;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) return "";
    if (!SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0)) return "";
    if (!CreatePipe(&hStdInRead, &hStdInWrite, &sa, 0)) return "";
    if (!SetHandleInformation(hStdInWrite, HANDLE_FLAG_INHERIT, 0)) return "";

    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdError = hStdOutWrite;
    si.hStdOutput = hStdOutWrite;
    si.hStdInput = hStdInRead;
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    std::string deviceId = GetFirstAdbDeviceId();
    std::string fullCommand = deviceId.empty()
        ? ".\\HD-Adb shell"
        : ".\\HD-Adb -s " + deviceId + " shell";

    if (!CreateProcessA(NULL, (LPSTR)fullCommand.c_str(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        return "";
    }

    CloseHandle(hStdOutWrite);
    CloseHandle(hStdInRead);

    DWORD written;
    std::string commands = firstCommand + "\n" + secondCommand + "\n";
    if (!WriteFile(hStdInWrite, commands.c_str(), commands.length(), &written, NULL)) return "";
    CloseHandle(hStdInWrite);

    CHAR buffer[128];
    DWORD read;
    std::string output;
    while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &read, NULL) && read > 0) {
        buffer[read] = '\0';
        output += buffer;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hStdOutRead);

    return ExtraerLibAddressZ(output);
}

std::string ShellGetAddressNoSuZ(const std::string& comand) {
    HANDLE hStdOutRead, hStdOutWrite;
    HANDLE hStdInRead, hStdInWrite;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) return "";
    if (!SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0)) return "";
    if (!CreatePipe(&hStdInRead, &hStdInWrite, &sa, 0)) return "";
    if (!SetHandleInformation(hStdInWrite, HANDLE_FLAG_INHERIT, 0)) return "";

    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdError = hStdOutWrite;
    si.hStdOutput = hStdOutWrite;
    si.hStdInput = hStdInRead;
    si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    std::string deviceId = GetFirstAdbDeviceId();
    std::string fullCommand = deviceId.empty()
        ? ".\\HD-Adb shell \"getprop ro.secure ; /boot/android/android/system/xbin/bstk/su\""
        : ".\\HD-Adb -s " + deviceId + " shell \"getprop ro.secure ; /boot/android/android/system/xbin/bstk/su\"";

    if (!CreateProcessA(NULL, (LPSTR)fullCommand.c_str(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        return "";
    }

    CloseHandle(hStdOutWrite);
    CloseHandle(hStdInRead);

    DWORD written;
    std::string commands = comand + "\n";
    if (!WriteFile(hStdInWrite, commands.c_str(), commands.length(), &written, NULL)) return "";
    CloseHandle(hStdInWrite);

    CHAR buffer[128];
    DWORD read;
    std::string output;
    while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &read, NULL) && read > 0) {
        buffer[read] = '\0';
        output += buffer;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hStdOutRead);

    return ExtraerLibAddressZ(output);
}

// FunciÃ³n duplicada para libunity.so
std::string ShellGetAddressNoSuZ2(const std::string& comand) {
    HANDLE hStdOutRead, hStdOutWrite;
    HANDLE hStdInRead, hStdInWrite;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) return "";
    if (!SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0)) return "";
    if (!CreatePipe(&hStdInRead, &hStdInWrite, &sa, 0)) return "";
    if (!SetHandleInformation(hStdInWrite, HANDLE_FLAG_INHERIT, 0)) return "";

    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdError = hStdOutWrite;
    si.hStdOutput = hStdOutWrite;
    si.hStdInput = hStdInRead;
    si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    std::string deviceId = GetFirstAdbDeviceId();
    std::string fullCommand = deviceId.empty()
        ? ".\\HD-Adb shell \"getprop ro.secure ; /boot/android/android/system/xbin/bstk/su\""
        : ".\\HD-Adb -s " + deviceId + " shell \"getprop ro.secure ; /boot/android/android/system/xbin/bstk/su\"";

    if (!CreateProcessA(NULL, (LPSTR)fullCommand.c_str(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        return "";
    }

    CloseHandle(hStdOutWrite);
    CloseHandle(hStdInRead);

    DWORD written;
    std::string commands = comand + "\n";
    if (!WriteFile(hStdInWrite, commands.c_str(), commands.length(), &written, NULL)) return "";
    CloseHandle(hStdInWrite);

    CHAR buffer[128];
    DWORD read;
    std::string output;
    while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &read, NULL) && read > 0) {
        buffer[read] = '\0';
        output += buffer;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hStdOutRead);

    return ExtraerLibAddressZ(output);
}

void ComdADBZ(const std::string& command) {
    std::string deviceId = GetFirstAdbDeviceId();
    std::string fullCommand = deviceId.empty()
        ? ".\\HD-Adb shell " + command
        : ".\\HD-Adb -s " + deviceId + " shell " + command;

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.wShowWindow = SW_HIDE;
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcessA(NULL, const_cast<LPSTR>(fullCommand.c_str()), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        std::cerr << "Failed to start process. Error: " << GetLastError() << std::endl;
        return;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

/////////////////////////////////////////////


void KillProcessByFenix(const wchar_t* processNameW) {
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
                    wprintf(L"Proceso %s cerrado exitosamente.\n", processNameW);
                }
            }
        } while (Process32NextW(hSnap, &pe32));
    }
    CloseHandle(hSnap);
}




// Thread para inicializar ADB en background sin afectar FPS
static std::atomic<bool> adbThreadRunning = false;
static std::atomic<bool> adbInitialized = false;

/////////////////////////////////////////////////////



DWORD WINAPI ADBInitThread(LPVOID lpParam) {
    // Proceso ADB en thread separado para no causar tirones
    KillAdbZ();
    Sleep(50);

    ComdADBZ("kill-server");
    Sleep(100);

    ComdADBZ("start-server");
    Sleep(150);

    ComdADBZ("devices");
    Sleep(50);

    // Buscar libil2cpp.so
    std::string adbCmd = "cat /proc/$(pidof " + GamePackage + ")/maps | grep libil2cpp.so";
    std::string il2cppStr = ShellGetAddressNoSuZ(adbCmd.c_str());

    if (il2cppStr.empty()) {
        Sleep(200);
        il2cppStr = ShellGetAddressNoSuZ(adbCmd.c_str());
    }

    if (!il2cppStr.empty()) {
        Il2Cpp = uIntExtrZ(il2cppStr);
    }

    // Buscar libunity.so (independientemente de libil2cpp)
    std::string adbCmd2 = "cat /proc/$(pidof " + GamePackage + ")/maps | grep libunity.so";
    std::string libunityStr = ShellGetAddressNoSuZ2(adbCmd2.c_str());

    if (libunityStr.empty()) {
        Sleep(200);
        libunityStr = ShellGetAddressNoSuZ2(adbCmd2.c_str());
    }

    if (!libunityStr.empty()) {
        Libunity = uIntExtrZ2(libunityStr);
    }

    // KillProcessByFenix(L"HD-Adb.exe");
    // KillProcessByFenix(L"BstkSVC.exe");
    EspADB = "The Abd is active!";

    adbInitialized = true;
    adbThreadRunning = false;
    return 0;
}

//////////////////////////////////////////////////////////


// Iniciar ADB y obtener ventana del emulador - Sin Tirones, Sin Lag
HWND INJECTESPADB() {
    static bool initialized = false;
    static HWND cachedHwnd = NULL;

    // Return instantÃ¡neo si ya estÃ¡ listo
    if (initialized && cachedHwnd != NULL) {
        return cachedHwnd;
    }

    bool adbinject = true;
    if (!adbinject) {
        return NULL;
    }

    auto vmm = GetModuleHandleA("BstkVMM.dll");
    if (!vmm) {
        return NULL;
    }

    auto readFunc = (PGMPhysReadFunc)GetProcAddress(vmm, "PGMPhysRead");
    if (!readFunc) {
        return NULL;
    }

    // Hooks solo una vez
    static bool hooksInitialized = false;
    if (!hooksInitialized) {
        MH_Initialize();
        if (MH_CreateHook((LPVOID)readFunc, HookedPGMPhysRead, (LPVOID*)&ogPhysRead) != MH_OK) {
            return NULL;
        }
        if (MH_EnableHook((LPVOID)readFunc) != MH_OK) {
            return NULL;
        }
        hooksInitialized = true;
    }

    // Espera no bloqueante para vmPtr
    int attempts = 0;
    while (vmPtr == nullptr && attempts < 100) {
        Sleep(10);  // Sleep corto para no causar tirones
        attempts++;
    }

    if (vmPtr == nullptr) {
        return NULL;
    }

    ogCPU = (VMMGetCpuByIdFunc)GetProcAddress(vmm, "VMMGetCpuById");
    ogCast = (PGMPhysGCPtr2GCPhysFunc)GetProcAddress(vmm, "PGMPhysGCPtr2GCPhys");
    ogWrite = (PGMPhysSimpleWriteGCPhysFunc)GetProcAddress(vmm, "PGMPhysSimpleWriteGCPhys");

    if (!ogCPU || !ogCast || !ogWrite) {
        return NULL;
    }

    InitializeZ(vmPtr);
    CambiarDirectoryZ(GetExeDirectoryZ());

    // Iniciar thread ADB en background (no bloquea el ESP)
    if (!adbThreadRunning && !adbInitialized) {
        adbThreadRunning = true;
        HANDLE hThread = CreateThread(NULL, 0, ADBInitThread, NULL, 0, NULL);
        if (hThread) {
            CloseHandle(hThread);  // Dejar que corra en background
        }
    }


    cachedHwnd = FindRenderWindow();

    // Marcar como inicializado aunque ADB siga en background
    initialized = true;

    return cachedHwnd;
}


//////////////////////////////////////////////////////



bool GetBluestacksMiddlePos(POINT& middlePos) {

    HWND HwndEmul = NULL;
    HwndEmul = FindWindowA(NULL, "BlueStacks App Player");
    if (HwndEmul == NULL)
    {
        HwndEmul = FindWindowA(NULL, "MSI App Player");
        if (HwndEmul == NULL)
        {
            HwndEmul = FindWindowA(NULL, "BlueStacks");
            if (HwndEmul == NULL)
            {
                HwndEmul = FindWindowA(NULL, "App Player");
                if (HwndEmul == NULL)
                {
                    HwndEmul = FindWindowA(NULL, "Ultra God");
                    if (HwndEmul == NULL)
                    {
                        HwndEmul = FindWindowA(NULL, "e4vX Bs4");
                        if (HwndEmul == NULL)
                        {
                            HwndEmul = FindWindowA(NULL, "@cosmo_byte");
                            if (HwndEmul == NULL)
                            {

                            }
                            else
                            {

                            }
                        }
                        else
                        {

                        }
                    }
                    else
                    {

                    }
                }
                else
                {

                }
            }
            else
            {

            }
        }
        else
        {

        }
    }

    RECT clientRect;
    if (!GetClientRect(HwndEmul, &clientRect)) {
        return false;
    }

    middlePos.x = (clientRect.right - clientRect.left) / 2;
    middlePos.y = (clientRect.bottom - clientRect.top) / 2;

    ClientToScreen(HwndEmul, &middlePos);
    return true;
}
///////////////////////////////

