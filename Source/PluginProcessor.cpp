#include "PluginProcessor.h"
#include "PluginEditor.h"

VstTestPlaygroundAudioProcessor::VstTestPlaygroundAudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    // Get atomic pointers to the parameters for the audio thread
    gainParam = apvts.getRawParameterValue("GAIN");
    onOffParam = apvts.getRawParameterValue("ONOFF");

    // Set up the oscillator
    oscillator.setFrequency(440.0f); // A4 note
    oscillator.initialise([](float x) { return std::sin(x); }); // Sine wave
}

VstTestPlaygroundAudioProcessor::~VstTestPlaygroundAudioProcessor() = default;

juce::AudioProcessorValueTreeState::ParameterLayout VstTestPlaygroundAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("ONOFF", "On/Off", true));

    return { params.begin(), params.end() };
}

void VstTestPlaygroundAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec{ sampleRate, (juce::uint32)samplesPerBlock, 2 };
    oscillator.prepare(spec);
    gain.prepare(spec);
}

void VstTestPlaygroundAudioProcessor::releaseResources() {}

void VstTestPlaygroundAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Check the On/Off state. If off, we do nothing and return silence.
    if (onOffParam->load() < 0.5f)
        return;

    // Set the gain from our parameter
    gain.setGainLinear(gainParam->load());

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    
    // Process the audio
    oscillator.process(context);
    gain.process(context);
}

void VstTestPlaygroundAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void VstTestPlaygroundAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessorEditor* VstTestPlaygroundAudioProcessor::createEditor()
{
    return new VstTestPlaygroundAudioProcessorEditor(*this);
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VstTestPlaygroundAudioProcessor();
}