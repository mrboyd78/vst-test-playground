#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VstTestPlaygroundAudioProcessorEditor::VstTestPlaygroundAudioProcessorEditor (VstTestPlaygroundAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    gainSlider.setSliderStyle (juce::Slider::Rotary);
    gainSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    gainSlider.setPopupDisplayEnabled (true, false, this);
    gainSlider.setTextValueSuffix (" dB");
    addAndMakeVisible (&gainSlider);

    gainAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (processorRef.apvts, "gain", gainSlider));

    setSize (400, 300);
}

VstTestPlaygroundAudioProcessorEditor::~VstTestPlaygroundAudioProcessorEditor()
{
}

//==============================================================================
void VstTestPlaygroundAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void VstTestPlaygroundAudioProcessorEditor::resized()
{
    gainSlider.setBounds (40, 40, 150, 150);
}
