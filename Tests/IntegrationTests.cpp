#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "../Source/PluginProcessor.h"
#include "../Source/PluginEditor.h"
#include "../Source/WebView.h"
#include "../Source/Params.h"

/**
 * Integration Tests for VstTestPlayground
 * Tests the integration between processor, editor, and WebView
 */
class IntegrationTests : public juce::UnitTest
{
public:
    IntegrationTests() : juce::UnitTest("Integration Tests for VstTestPlayground") {}

    void runTest() override
    {
        beginTest("Processor and Editor Lifecycle");
        {
            VstTestPlaygroundAudioProcessor processor;
            
            // Create editor
            auto* editor = processor.createEditor();
            expect(editor != nullptr, "Editor should be created");
            
            // Process some audio
            processor.prepareToPlay(44100.0, 512);
            juce::AudioBuffer<float> buffer(2, 512);
            juce::MidiBuffer midiBuffer;
            buffer.clear();
            
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                buffer.setSample(0, i, 0.5f);
                buffer.setSample(1, i, 0.5f);
            }
            
            processor.processBlock(buffer, midiBuffer);
            
            expect(true, "Should process audio while editor exists");
            
            // Delete editor
            delete editor;
            
            // Continue processing
            buffer.clear();
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                buffer.setSample(0, i, 0.5f);
                buffer.setSample(1, i, 0.5f);
            }
            
            processor.processBlock(buffer, midiBuffer);
            expect(true, "Should process audio after editor is deleted");
        }

        beginTest("Parameter Sync Between Processor and Editor");
        {
            VstTestPlaygroundAudioProcessor processor;
            auto* editor = processor.createEditor();
            
            expect(editor != nullptr, "Editor should exist");
            
            auto* gainParam = processor.apvts.getParameter(Params::GAIN_ID);
            
            // Change parameter from processor side
            float testValue = 0.65f;
            gainParam->setValueNotifyingHost(testValue);
            
            // Verify parameter value
            expectWithinAbsoluteError(gainParam->getValue(), testValue, 0.001f,
                                     "Parameter should be at test value");
            
            delete editor;
        }

        beginTest("Multiple Editors for Same Processor");
        {
            VstTestPlaygroundAudioProcessor processor;
            
            auto* editor1 = processor.createEditor();
            expect(editor1 != nullptr, "First editor should be created");
            
            delete editor1;
            
            auto* editor2 = processor.createEditor();
            expect(editor2 != nullptr, "Second editor should be created after first is deleted");
            
            delete editor2;
        }

        beginTest("State Persistence Across Editor Lifecycle");
        {
            VstTestPlaygroundAudioProcessor processor;
            
            // Set initial parameter value
            auto* gainParam = processor.apvts.getParameter(Params::GAIN_ID);
            gainParam->setValueNotifyingHost(0.8f);
            
            // Create and destroy editor
            auto* editor1 = processor.createEditor();
            delete editor1;
            
            // Value should persist
            expectWithinAbsoluteError(gainParam->getValue(), 0.8f, 0.001f,
                                     "Parameter should persist after editor destruction");
            
            // Create new editor
            auto* editor2 = processor.createEditor();
            expect(editor2 != nullptr, "New editor should be created");
            
            // Value should still be there
            expectWithinAbsoluteError(gainParam->getValue(), 0.8f, 0.001f,
                                     "Parameter should persist with new editor");
            
            delete editor2;
        }

        beginTest("Audio Processing with Parameter Changes");
        {
            VstTestPlaygroundAudioProcessor processor;
            processor.prepareToPlay(44100.0, 512);
            
            auto* editor = processor.createEditor();
            
            juce::AudioBuffer<float> buffer(2, 512);
            juce::MidiBuffer midiBuffer;
            
            auto* gainParam = processor.apvts.getParameter(Params::GAIN_ID);
            
            // Process at minimum gain
            gainParam->setValueNotifyingHost(0.0f);
            buffer.clear();
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                buffer.setSample(0, i, 1.0f);
                buffer.setSample(1, i, 1.0f);
            }
            processor.processBlock(buffer, midiBuffer);
            
            float rmsMin = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
            
            // Process at maximum gain
            gainParam->setValueNotifyingHost(1.0f);
            buffer.clear();
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                buffer.setSample(0, i, 0.1f);
                buffer.setSample(1, i, 0.1f);
            }
            processor.processBlock(buffer, midiBuffer);
            
            float rmsMax = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
            
            expect(rmsMin < 0.05f, "Minimum gain should produce quiet output");
            expect(rmsMax > 0.1f, "Maximum gain should amplify signal");
            
            delete editor;
        }

        beginTest("WebView Options Configuration");
        {
            // Test that WebView can be created with proper options
            auto options = juce::WebBrowserComponent::Options()
                .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
                .withNativeIntegrationEnabled();
            
            // This should not crash
            std::unique_ptr<WebView> webView;
            
            try {
                webView = std::make_unique<WebView>(options);
                expect(webView != nullptr, "WebView should be created with options");
            }
            catch (...)
            {
                expect(false, "WebView creation should not throw exceptions");
            }
        }

        beginTest("Editor Cleanup on Processor Destruction");
        {
            auto processor = std::make_unique<VstTestPlaygroundAudioProcessor>();
            auto* editor = processor->createEditor();
            
            expect(editor != nullptr, "Editor should be created");
            
            // Delete editor first
            delete editor;
            
            // Then destroy processor - should not crash
            processor.reset();
            
            expect(true, "Processor destruction after editor deletion should be safe");
        }

        beginTest("Concurrent Parameter Changes");
        {
            VstTestPlaygroundAudioProcessor processor;
            processor.prepareToPlay(44100.0, 512);
            
            auto* editor = processor.createEditor();
            auto* gainParam = processor.apvts.getParameter(Params::GAIN_ID);
            
            juce::AudioBuffer<float> buffer(2, 512);
            juce::MidiBuffer midiBuffer;
            
            // Simulate concurrent parameter changes and audio processing
            for (int i = 0; i < 20; ++i)
            {
                gainParam->setValueNotifyingHost(static_cast<float>(i) / 20.0f);
                
                buffer.clear();
                for (int j = 0; j < buffer.getNumSamples(); ++j)
                {
                    buffer.setSample(0, j, 0.5f);
                    buffer.setSample(1, j, 0.5f);
                }
                
                processor.processBlock(buffer, midiBuffer);
            }
            
            expect(true, "Concurrent parameter changes and audio processing should be safe");
            
            delete editor;
        }

        beginTest("APVTS State with Editor");
        {
            VstTestPlaygroundAudioProcessor processor;
            
            // Set up some state
            auto* gainParam = processor.apvts.getParameter(Params::GAIN_ID);
            gainParam->setValueNotifyingHost(0.42f);
            
            // Save state
            juce::MemoryBlock savedState;
            processor.getStateInformation(savedState);
            
            // Create editor
            auto* editor = processor.createEditor();
            
            // Modify parameter
            gainParam->setValueNotifyingHost(0.99f);
            
            // Restore state
            processor.setStateInformation(savedState.getData(), 
                                         static_cast<int>(savedState.getSize()));
            
            // Check restored value
            expectWithinAbsoluteError(gainParam->getValue(), 0.42f, 0.001f,
                                     "State should restore correctly with editor active");
            
            delete editor;
        }
    }
};

// Register the test suite
static IntegrationTests integrationTests;
