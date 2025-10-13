#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "../Source/PluginProcessor.h"
#include "../Source/Params.h" // This file does not exist yet.

class ParameterTests : public juce::UnitTest
{
public:
    ParameterTests() : juce::UnitTest("Parameter Tests for VstTestPlayground") {}

    void runTest() override
    {
        beginTest("Parameter Existence");

        VstTestPlaygroundAudioProcessor processor;
        auto& apvts = processor.apvts;

        // This will fail to compile because Params::GAIN_ID does not exist.
        auto* gainParam = apvts.getParameter(Params::GAIN_ID);
        expect(gainParam != nullptr, "Gain parameter should exist.");
    }
};

// This static instance tells the JUCE test runner to run this test.
static ParameterTests parameterTests;
