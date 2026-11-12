#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <imgui.h>
#include "../IFeature.h"
#include "../../Core/Config.h"
#include "../../Core/Offsets.h"
#include "../../Esp/EspBox3d.h"

namespace Features {
namespace Visuals {

    class EspBox3dFeature : public IFeature {
    public:
        const char* GetName() const override {
            return "ESP Box 3D";
        }

        const char* GetDescription() const override {
            return "Dibuja una caja tridimensional (cubo wireframe) alrededor de cada jugador proyectada en pantalla";
        }

        FeatureCategory GetCategory() const override {
            return FeatureCategory::Visuals;
        }

        bool IsEnabled() const override {
            auto& cfg = Core::GetConfig();
            return cfg.EspBox3dEnabled;
        }

        void SetEnabled(bool enabled) override {
            Core::GetConfig().EspBox3dEnabled = enabled;
        }

        void OnRender() override {
            if (!IsEnabled()) return;

            // Ejecutar el motor de renderizado de caja 3D
            ESP_Box3D();
        }
    };

} // namespace Visuals
} // namespace Features
