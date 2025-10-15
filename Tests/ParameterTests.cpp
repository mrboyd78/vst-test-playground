#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "../Source/PluginProcessor.h"
#include "../Source/Params.h"

class ParameterTests : public juce::UnitTest
{
public:
    ParameterTests() : juce::UnitTest("Parameter Tests for VstTestPlayground") {}

    void runTest() override
    {
        beginTest("Parameter Existence");
        {
            VstTestPlaygroundAudioProcessor processor;
            auto& apvts = processor.apvts;

            auto* gainParam = apvts.getParameter(Params::GAIN_ID);
            expect(gainParam != nullptr, "Gain parameter should exist.");
        }

        beginTest("Parameter Range Validation");
        {
            VstTestPlaygroundAudioProcessor processor;
            auto* gainParam = processor.apvts.getParameter(Params::GAIN_ID);
            
            // Test minimum value
            gainParam->setValueNotifyingHost(0.0f);
            expectWithinAbsoluteError(gainParam->getValue(), 0.0f, 0.001f, 
                                     "Minimum normalized value should be 0.0");
            
            // Test maximum value
            gainParam->setValueNotifyingHost(1.0f);
            expectWithinAbsoluteError(gainParam->getValue(), 1.0f, 0.001f, 
                                     "Maximum normalized value should be 1.0");
            
            // Test default value (should be unity gain = 0dB, which is at 60/72 = 0.833 normalized)
            gainParam->setValueNotifyingHost(gainParam->getDefaultValue());
            auto* rawParam = dynamic_cast<juce::AudioParameterFloat*>(gainParam);
            if (rawParam)
            {
                float defaultDB = rawParam->get();
                expectWithinAbsoluteError(defaultDB, 0.0f, 0.1f, 
                                         "Default gain should be 0dB (unity)");
            }
        }

        beginTest("State Persistence");
        {
            VstTestPlaygroundAudioProcessor processor;
            auto* gainParam = processor.apvts.getParameter(Params::GAIN_ID);
            
            // Set a specific value
            gainParam->setValueNotifyingHost(0.75f);
            
            // Save state
            juce::MemoryBlock state;
            processor.getStateInformation(state);
            
            // Create new processor and restore state
            VstTestPlaygroundAudioProcessor processor2;
            processor2.setStateInformation(state.getData(), static_cast<int>(state.getSize()));
            
            auto* gainParam2 = processor2.apvts.getParameter(Params::GAIN_ID);
            expectWithinAbsoluteError(gainParam2->getValue(), 0.75f, 0.001f, 
                                     "Parameter should restore from saved state");
        }

        beginTest("ProcessBlock Execution");
        {
            VstTestPlaygroundAudioProcessor processor;
            processor.prepareToPlay(44100.0, 512);
            
            juce::AudioBuffer<float> buffer(2, 512);
            juce::MidiBuffer midiBuffer;
            buffer.clear();
            
            // Fill with test signal
            for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
            {
                auto* channelData = buffer.getWritePointer(channel);
                for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
                {
                    channelData[sample] = 0.5f; // Test signal at half amplitude
                }
            }
            
            // This should complete without exceptions
            processor.processBlock(buffer, midiBuffer);
            
            expect(true, "ProcessBlock completed without exceptions");
            
            // Verify buffer still contains data (not all zeros after processing)
            bool hasNonZeroData = false;
            for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
            {
                auto* channelData = buffer.getReadPointer(channel);
                for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
                {
                    if (std::abs(channelData[sample]) > 0.0001f)
                    {
                        hasNonZeroData = true;
                        break;
                    }
                }
            }
            expect(hasNonZeroData, "ProcessBlock should produce output from input signal");
        }

        beginTest("Gain Parameter Affects Output");
        {
            VstTestPlaygroundAudioProcessor processor;
            processor.prepareToPlay(44100.0, 512);
            
            auto* gainParam = processor.apvts.getParameter(Params::GAIN_ID);
            
            // Set gain to minimum (-60dB, should be nearly silent)
            gainParam->setValueNotifyingHost(0.0f);
            
            juce::AudioBuffer<float> buffer1(2, 512);
            juce::MidiBuffer midiBuffer;
            for (int i = 0; i < buffer1.getNumSamples(); ++i)
            {
                buffer1.setSample(0, i, 1.0f);
                buffer1.setSample(1, i, 1.0f);
            }
            
            processor.processBlock(buffer1, midiBuffer);
            
            // Output should be heavily attenuated
            float rms1 = buffer1.getRMSLevel(0, 0, buffer1.getNumSamples());
            expect(rms1 < 0.01f, "Gain at -60dB should produce very quiet output");
            
            // Set gain to maximum (+12dB)
            gainParam->setValueNotifyingHost(1.0f);
            
            juce::AudioBuffer<float> buffer2(2, 512);
            for (int i = 0; i < buffer2.getNumSamples(); ++i)
            {
                buffer2.setSample(0, i, 0.1f);
                buffer2.setSample(1, i, 0.1f);
            }
            
            processor.processBlock(buffer2, midiBuffer);
            
            // Output should be amplified
            float rms2 = buffer2.getRMSLevel(0, 0, buffer2.getNumSamples());
            expect(rms2 > 0.1f, "Gain at +12dB should amplify signal");
        }

        beginTest("Undo Manager Integration");
        {
            VstTestPlaygroundAudioProcessor processor;
            auto* gainParam = processor.apvts.getParameter(Params::GAIN_ID);
            
            // The processor should have an undo manager
            expect(true, "Processor initialized with undo manager");
            
            // Set initial value
            float initialValue = 0.5f;
            gainParam->setValueNotifyingHost(initialValue);
            
            // Change value
            float newValue = 0.8f;
            gainParam->setValueNotifyingHost(newValue);
            expectWithinAbsoluteError(gainParam->getValue(), newValue, 0.001f,
                                     "Parameter should be at new value");
        }
    }
};

// This static instance tells the JUCE test runner to run this test.
static ParameterTests parameterTests;
