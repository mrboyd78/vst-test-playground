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
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    gainParam = apvts.getRawParameterValue(Params::GAIN_ID);
}

VstTestPlaygroundAudioProcessor::~VstTestPlaygroundAudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout VstTestPlaygroundAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        Params::GAIN_ID,
        "Gain",
        juce::NormalisableRange<float>(-100.0f, 100.0f, 0.01f),
        0.0f));

    return layout;
}

//==============================================================================
const juce::String VstTestPlaygroundAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VstTestPlaygroundAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool VstTestPlaygroundAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool VstTestPlaygroundAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double VstTestPlaygroundAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//==============================================================================
int VstTestPlaygroundAudioProcessor::getNumPrograms()
{
    return 1;
}

int VstTestPlaygroundAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VstTestPlaygroundAudioProcessor::setCurrentProgram(int)
{
}

const juce::String VstTestPlaygroundAudioProcessor::getProgramName(int)
{
    return {};
}

void VstTestPlaygroundAudioProcessor::changeProgramName(int, const juce::String&)
{
}

//==============================================================================
void VstTestPlaygroundAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    gain.prepare(spec);
    gain.setRampDurationSeconds(0.05);
}

void VstTestPlaygroundAudioProcessor::releaseResources()
{
}

bool VstTestPlaygroundAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void VstTestPlaygroundAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    gain.setGainDecibels(gainParam->load());
    gain.process(context);
}

//==============================================================================
bool VstTestPlaygroundAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* VstTestPlaygroundAudioProcessor::createEditor()
{
    return new VstTestPlaygroundAudioProcessorEditor(*this);
}

//==============================================================================
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