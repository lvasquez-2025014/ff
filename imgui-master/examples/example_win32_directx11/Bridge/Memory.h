#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <cstdint>
#include <vector>
#include <string>
#include <unordered_map>
#include "../Math/Vector.h"

namespace Bridge {
namespace Memory {

    // Virtual Machine Memory Pointers & Callbacks
    inline void* vmPtr = nullptr;
    inline void* pVMAddr = nullptr;
    inline void* cpuAddr = nullptr;
    inline std::unordered_map<uintptr_t, uintptr_t> CacheZ;

    typedef int(__cdecl* PGMPhysReadFunc)(void*, uintptr_t, void*, size_t);
    typedef int(__cdecl* PGMPhysSimpleWriteGCPhysFunc)(void*, uintptr_t, void*, size_t);
    typedef int(__cdecl* PGMPhysGCPtr2GCPhysFunc)(void*, uintptr_t, uintptr_t*);
    typedef void* (__cdecl* VMMGetCpuByIdFunc)(void*, int);

    inline PGMPhysReadFunc ogPhysRead = nullptr;
    inline VMMGetCpuByIdFunc ogCPU = nullptr;
    inline PGMPhysGCPtr2GCPhysFunc ogCast = nullptr;
    inline PGMPhysSimpleWriteGCPhysFunc ogWrite = nullptr;

    inline int __cdecl HookedPGMPhysRead(void* pVM, uintptr_t GCPhys, void* pvBuf, size_t cbRead) {
        if (!vmPtr) vmPtr = pVM;
        return ogPhysRead(pVM, GCPhys, pvBuf, cbRead);
    }

    inline int zHookWrite(void* pVM, uintptr_t GCPhys, void* pvBuf, size_t cbRead) {
        return ogWrite(pVM, GCPhys, pvBuf, cbRead);
    }

    inline int zHookRead(void* pVM, uintptr_t GCPhys, void* pvBuf, size_t cbRead) {
        return ogPhysRead(pVM, GCPhys, pvBuf, cbRead);
    }

    inline void* CPU(void* pVM, int cpuId) {
        return ogCPU ? ogCPU(pVM, cpuId) : nullptr;
    }

    inline int Cast(void* pVCpu, uintptr_t address, uintptr_t* physAddress) {
        return ogCast ? ogCast(pVCpu, address, physAddress) : -1;
    }

    inline void Initialize(void* pVM) {
        pVMAddr = pVM;
        if (pVM && ogCPU)
            cpuAddr = CPU(pVM, 0);
        CacheZ.clear();
    }

    inline bool ConvertZ(uintptr_t address, uintptr_t& phys) {
        auto it = CacheZ.find(address);
        if (it != CacheZ.end()) {
            phys = it->second;
            return true;
        }

        if (!pVMAddr || !ogCPU || !ogCast) return false;
        if (!cpuAddr) {
            cpuAddr = CPU(pVMAddr, 0);
        }
        if (!cpuAddr) return false;

        int status = Cast(cpuAddr, address, &phys);
        if (status != 0) return false;

        CacheZ[address] = phys;
        return true;
    }

    template<typename T>
    inline bool Read(uintptr_t address, T& data) {
        uintptr_t physAddress;
        return ConvertZ(address, physAddress) && (zHookRead(pVMAddr, physAddress, &data, sizeof(T)) == 0);
    }

    template<typename T>
    inline bool ReadArray(uintptr_t address, std::vector<T>& array) {
        uintptr_t physAddress;
        if (!ConvertZ(address, physAddress)) return false;
        return zHookRead(pVMAddr, physAddress, array.data(), sizeof(T) * array.size()) == 0;
    }

    template<typename T>
    inline void Write(uintptr_t address, const T& value) {
        uintptr_t physAddress;
        if (ConvertZ(address, physAddress)) {
            zHookWrite(pVMAddr, physAddress, (void*)&value, sizeof(T));
        }
    }

    // Unity / FreeFire Transform & Bone Matrix structures
    struct TMatrix {
        Math::Vector4 position;
        Math::Quaternion rotation;
        Math::Vector4 scale;
    };

    inline bool GetPosition(uint32_t transform, Math::Vector3& pos) {
        pos = Math::Vector3::Zero();
        uint32_t transformObjValue;
        if (!Read(transform + 0x8, transformObjValue)) return false;

        uint32_t indexValue;
        if (!Read(transformObjValue + 0x24, indexValue)) return false;

        uint32_t matrixValue;
        if (!Read(transformObjValue + 0x20, matrixValue)) return false;

        uint32_t matrixListValue;
        if (!Read(matrixValue + 0x18, matrixListValue)) return false;

        uint32_t matrixIndicesValue;
        if (!Read(matrixValue + 0x1C, matrixIndicesValue)) return false;

        Math::Vector3 resultValue;
        if (!Read(indexValue * 0x30 + matrixListValue, resultValue)) return false;

        int maxTries = 50;
        int tries = 0;
        int transformIndexValue;
        if (!Read((uint32_t)((indexValue * 0x4) + matrixIndicesValue), transformIndexValue)) return false;

        while (transformIndexValue >= 0) {
            tries++;
            if (tries == maxTries) break;
            TMatrix tMatrixValue;
            if (!Read((uint32_t)(0x30 * transformIndexValue + matrixListValue), tMatrixValue)) return false;

            float rotX = tMatrixValue.rotation.x;
            float rotY = tMatrixValue.rotation.y;
            float rotZ = tMatrixValue.rotation.z;
            float rotW = tMatrixValue.rotation.w;
            float scaleX = resultValue.x * tMatrixValue.scale.x;
            float scaleY = resultValue.y * tMatrixValue.scale.y;
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

            if (!Read((uint32_t)(transformIndexValue * 0x4 + matrixIndicesValue), transformIndexValue)) return false;
        }
        pos = resultValue;
        return (tries != maxTries);
    }

    inline bool GetNodePosition(uint32_t nodeTransform, Math::Vector3& result) {
        uint32_t transformValue;
        return Read(nodeTransform + 0x8, transformValue) && GetPosition(transformValue, result);
    }

} // namespace Memory
} // namespace Bridge
