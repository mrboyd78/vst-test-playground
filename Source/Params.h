#pragma once
#include <juce_core/juce_core.h>

namespace Params
{
    struct ParameterMetadata
    {
        const juce::String id;
        const juce::String name;
        const float minValue;
        const float maxValue;
        const float defaultValue;
    };

    static const ParameterMetadata gain = { "gain", "Gain", -60.0f, 12.0f, 0.0f };

    inline const juce::String GAIN_ID { "gain" };
}
