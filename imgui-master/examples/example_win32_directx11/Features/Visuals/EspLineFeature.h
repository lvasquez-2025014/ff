#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <imgui.h>
#include "../IFeature.h"
#include "../../Core/Config.h"
#include "../../Core/Offsets.h"
#include "../../Esp/Esp_line.h"

namespace Features {
namespace Visuals {

    class EspLineFeature : public IFeature {
    public:
        const char* GetName() const override {
            return "ESP Lineas Cabeza";
        }

        const char* GetDescription() const override {
            return "Traza linea superior hacia la posicion de la cabeza de cada jugador en tiempo real";
        }

        FeatureCategory GetCategory() const override {
            return FeatureCategory::Visuals;
        }

        bool IsEnabled() const override {
            auto& cfg = Core::GetConfig();
            return cfg.EspLineEnabled || cfg.EspBoxEnabled || cfg.EspHealthEnabled || cfg.EspSkeletonEnabled || cfg.EspNameEnabled;
        }

        void SetEnabled(bool enabled) override {
            Core::GetConfig().EspLineEnabled = enabled;
            EspLineZ = enabled;
        }

        void OnRender() override {
            auto& cfg = Core::GetConfig();
            if (!IsEnabled())
                return;

            // Sincronizar todos los ajustes hacia el motor de renderizado ESP verificado
            EspLineZ = cfg.EspLineEnabled;
            EspBox = cfg.EspBoxEnabled;
            ESPBox = cfg.EspBoxEnabled;
            ESPHealth = cfg.EspHealthEnabled;
            ESPBones = cfg.EspSkeletonEnabled;
            ESPNameZ = cfg.EspNameEnabled;
            ShowTeam = cfg.ShowTeam;
            lineThicknessBase = cfg.LineThickness;
            lineColorEnemy = cfg.ColorEnemy;
            lineColorTeam = cfg.ColorTeam;

            // Ejecutar el motor de renderizado de ESP verificado
            ESP_line();
        }
    };

} // namespace Visuals
} // namespace Features
