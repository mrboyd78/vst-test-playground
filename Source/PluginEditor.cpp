#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Params.h"

//==============================================================================
VstTestPlaygroundAudioProcessorEditor::VstTestPlaygroundAudioProcessorEditor (VstTestPlaygroundAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    setLookAndFeel(&customLookAndFeel);

    gainSlider.setSliderStyle (juce::Slider::Rotary);
    gainSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    gainSlider.setPopupDisplayEnabled (true, false, this);
    gainSlider.setTextValueSuffix (" dB");
    addAndMakeVisible (&gainSlider);

    gainAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (processorRef.apvts, Params::GAIN_ID, gainSlider));

    setSize (400, 300);
}

VstTestPlaygroundAudioProcessorEditor::~VstTestPlaygroundAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void VstTestPlaygroundAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void VstTestPlaygroundAudioProcessorEditor::resized()
{
    juce::FlexBox flexBox;
    flexBox.flexWrap = juce::FlexBox::Wrap::wrap;
    flexBox.justifyContent = juce::FlexBox::JustifyContent::center;
    flexBox.alignContent = juce::FlexBox::AlignContent::center;

    flexBox.items.add(juce::FlexItem(gainSlider).withFlex(1.0f));

    flexBox.performLayout(getLocalBounds());
}
