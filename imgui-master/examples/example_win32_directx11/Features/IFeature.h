#pragma once
#include <string>

namespace Features {

    enum class FeatureCategory {
        Visuals,
        Exploits,
        Combat,
        Misc
    };

    inline const char* CategoryToString(FeatureCategory category) {
        switch (category) {
            case FeatureCategory::Visuals:  return "Visuales";
            case FeatureCategory::Exploits: return "Exploits";
            case FeatureCategory::Combat:   return "Combate";
            case FeatureCategory::Misc:     return "Varios";
            default:                        return "General";
        }
    }

    class IFeature {
    public:
        virtual ~IFeature() = default;

        // Metadata
        virtual const char* GetName() const = 0;
        virtual const char* GetDescription() const = 0;
        virtual FeatureCategory GetCategory() const = 0;

        // State control
        virtual bool IsEnabled() const = 0;
        virtual void SetEnabled(bool enabled) = 0;

        // Lifecycle hooks
        virtual void OnRender() {}
        virtual void OnUpdate() {}
    };

} // namespace Features
