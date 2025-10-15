#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Params.h"

//==============================================================================
VstTestPlaygroundAudioProcessor::VstTestPlaygroundAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         ),
      apvts(*this, &undoManager, "Parameters", createParameterLayout())
{
    gainParameter = apvts.getRawParameterValue(Params::GAIN_ID);
}

juce::AudioProcessorValueTreeState::ParameterLayout VstTestPlaygroundAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        Params::gain.id,
        Params::gain.name,
        juce::NormalisableRange<float>(
            Params::gain.minValue,
            Params::gain.maxValue,
            0.01f,
            1.0f),
        Params::gain.defaultValue));

    return layout;
}

void VstTestPlaygroundAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    gain.prepare(spec);
    gain.setRampDurationSeconds(0.05);
    
    // Initialize gain to current parameter value
    previousGainDB = gainParameter->load();
    gain.setGainDecibels(previousGainDB);
}

void VstTestPlaygroundAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Only update gain when parameter changes (avoid repeated calculations)
    float currentGainDB = gainParameter->load();
    if (!juce::approximatelyEqual(currentGainDB, previousGainDB))
    {
        gain.setGainDecibels(currentGainDB);
        previousGainDB = currentGainDB;
    }

    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    gain.process(context);
}

juce::AudioProcessorEditor* VstTestPlaygroundAudioProcessor::createEditor()
{
    return new VstTestPlaygroundAudioProcessorEditor(*this);
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

    if (xmlState != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VstTestPlaygroundAudioProcessor();
}