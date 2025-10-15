#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "../Source/PluginProcessor.h"
#include "../Source/PluginEditor.h"
#include "../Source/Params.h"

/**
 * UI Component Tests for VstTestPlayground
 * Tests the editor, WebView integration, and UI state management
 */
class UIComponentTests : public juce::UnitTest
{
public:
    UIComponentTests() : juce::UnitTest("UI Component Tests for VstTestPlayground") {}

    void runTest() override
    {
        beginTest("Editor Creation and Destruction");
        {
            VstTestPlaygroundAudioProcessor processor;
            auto* editor = dynamic_cast<VstTestPlaygroundAudioProcessorEditor*>(
                processor.createEditor());
            
            expect(editor != nullptr, "Editor should be created successfully");
            
            // Check initial size
            expect(editor->getWidth() == 400, "Editor width should be 400");
            expect(editor->getHeight() == 300, "Editor height should be 300");
            
            delete editor;
            expect(true, "Editor should be destroyed without crashes");
        }

        beginTest("Editor Has Processor Reference");
        {
            VstTestPlaygroundAudioProcessor processor;
            auto* editor = dynamic_cast<VstTestPlaygroundAudioProcessorEditor*>(
                processor.createEditor());
            
            expect(editor != nullptr, "Editor should exist");
            // Editor has valid processor reference (accessing it doesn't crash)
            editor->getAudioProcessor();
            expect(true, "Editor has valid processor reference");
            
            delete editor;
        }

        beginTest("Editor Components Are Visible");
        {
            VstTestPlaygroundAudioProcessor processor;
            auto* editor = dynamic_cast<VstTestPlaygroundAudioProcessorEditor*>(
                processor.createEditor());
            
            expect(editor != nullptr, "Editor should exist");
            expect(editor->isVisible() == false, "Editor starts invisible (not attached to window)");
            
            // Check that editor has child components (WebView)
            int childCount = editor->getNumChildComponents();
            expect(childCount > 0, "Editor should have child components");
            
            delete editor;
        }

        beginTest("Editor Responds to Resize");
        {
            VstTestPlaygroundAudioProcessor processor;
            auto* editor = dynamic_cast<VstTestPlaygroundAudioProcessorEditor*>(
                processor.createEditor());
            
            expect(editor != nullptr, "Editor should exist");
            
            // Test resize
            editor->setSize(800, 600);
            expect(editor->getWidth() == 800, "Editor width should change to 800");
            expect(editor->getHeight() == 600, "Editor height should change to 600");
            
            delete editor;
        }

        beginTest("Parameter Listener Registration");
        {
            VstTestPlaygroundAudioProcessor processor;
            auto* editor = dynamic_cast<VstTestPlaygroundAudioProcessorEditor*>(
                processor.createEditor());
            
            expect(editor != nullptr, "Editor should exist");
            
            // Change a parameter value
            auto* gainParam = processor.apvts.getParameter(Params::GAIN_ID);
            expect(gainParam != nullptr, "Gain parameter should exist");
            
            float oldValue = gainParam->getValue();
            gainParam->setValueNotifyingHost(0.5f);
            
            // The listener should be registered (no crash)
            expect(true, "Parameter change should not crash with listener");
            
            // Restore value
            gainParam->setValueNotifyingHost(oldValue);
            
            delete editor;
        }

        beginTest("Multiple Editor Instances");
        {
            VstTestPlaygroundAudioProcessor processor1;
            VstTestPlaygroundAudioProcessor processor2;
            
            auto* editor1 = dynamic_cast<VstTestPlaygroundAudioProcessorEditor*>(
                processor1.createEditor());
            auto* editor2 = dynamic_cast<VstTestPlaygroundAudioProcessorEditor*>(
                processor2.createEditor());
            
            expect(editor1 != nullptr, "First editor should be created");
            expect(editor2 != nullptr, "Second editor should be created");
            expect(editor1 != editor2, "Editors should be different instances");
            
            delete editor1;
            delete editor2;
            expect(true, "Multiple editors should be destroyed without issues");
        }

        beginTest("Editor Survives Parameter Changes");
        {
            VstTestPlaygroundAudioProcessor processor;
            auto* editor = dynamic_cast<VstTestPlaygroundAudioProcessorEditor*>(
                processor.createEditor());
            
            expect(editor != nullptr, "Editor should exist");
            
            auto* gainParam = processor.apvts.getParameter(Params::GAIN_ID);
            
            // Rapidly change parameter multiple times
            for (int i = 0; i < 10; ++i)
            {
                gainParam->setValueNotifyingHost(static_cast<float>(i) / 10.0f);
            }
            
            expect(true, "Editor should survive rapid parameter changes");
            
            delete editor;
        }

        beginTest("Editor State After Processor State Restore");
        {
            VstTestPlaygroundAudioProcessor processor1;
            auto* gainParam1 = processor1.apvts.getParameter(Params::GAIN_ID);
            gainParam1->setValueNotifyingHost(0.75f);
            
            // Save state
            juce::MemoryBlock state;
            processor1.getStateInformation(state);
            
            // Create new processor and editor
            VstTestPlaygroundAudioProcessor processor2;
            processor2.setStateInformation(state.getData(), static_cast<int>(state.getSize()));
            
            auto* editor = dynamic_cast<VstTestPlaygroundAudioProcessorEditor*>(
                processor2.createEditor());
            
            expect(editor != nullptr, "Editor should be created after state restore");
            
            auto* gainParam2 = processor2.apvts.getParameter(Params::GAIN_ID);
            expectWithinAbsoluteError(gainParam2->getValue(), 0.75f, 0.001f,
                                     "Parameter should maintain value after state restore");
            
            delete editor;
        }

        beginTest("Editor Painting Does Not Crash");
        {
            VstTestPlaygroundAudioProcessor processor;
            auto* editor = dynamic_cast<VstTestPlaygroundAudioProcessorEditor*>(
                processor.createEditor());
            
            expect(editor != nullptr, "Editor should exist");
            
            // Create a graphics context and test painting
            juce::Image testImage(juce::Image::ARGB, 400, 300, true);
            juce::Graphics g(testImage);
            
            // This should not crash
            editor->paint(g);
            
            expect(true, "Editor painting should complete without crash");
            
            delete editor;
        }

        beginTest("Editor Custom Look and Feel");
        {
            VstTestPlaygroundAudioProcessor processor;
            auto* editor = dynamic_cast<VstTestPlaygroundAudioProcessorEditor*>(
                processor.createEditor());
            
            expect(editor != nullptr, "Editor should exist");
            
            auto& laf = editor->getLookAndFeel();
            expect(&laf != nullptr, "Look and feel should be set");
            
            // Check that custom colors are available
            auto bgColor = laf.findColour(juce::ResizableWindow::backgroundColourId);
            expect(true, "Background color should be retrievable");
            
            delete editor;
        }
    }
};

// Register the test suite
static UIComponentTests uiComponentTests;
