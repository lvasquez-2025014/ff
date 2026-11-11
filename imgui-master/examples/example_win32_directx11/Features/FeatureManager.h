#pragma once
#include "IFeature.h"
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

namespace Features {

    class FeatureManager {
    private:
        std::vector<std::shared_ptr<IFeature>> m_features;

        FeatureManager() = default;

    public:
        static FeatureManager& Get() {
            static FeatureManager instance;
            return instance;
        }

        void RegisterFeature(std::shared_ptr<IFeature> feature) {
            if (feature) {
                m_features.push_back(feature);
            }
        }

        const std::vector<std::shared_ptr<IFeature>>& GetAllFeatures() const {
            return m_features;
        }

        std::shared_ptr<IFeature> FindFeature(const std::string& name) {
            for (auto& feat : m_features) {
                if (feat && feat->GetName() == name) {
                    return feat;
                }
            }
            return nullptr;
        }

        void RenderAll() {
            for (auto& feat : m_features) {
                if (feat && feat->IsEnabled()) {
                    feat->OnRender();
                }
            }
        }

        void UpdateAll() {
            for (auto& feat : m_features) {
                if (feat && feat->IsEnabled()) {
                    feat->OnUpdate();
                }
            }
        }
    };

} // namespace Features
