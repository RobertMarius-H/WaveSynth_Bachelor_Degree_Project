/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthSound.h"
#include "SynthVoice.h"

//==============================================================================
WaveSynthAudioProcessor::WaveSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts (*this, nullptr, "Parameters", createParams())
#endif
{
    
    for (int i = 0; i < numVoices; ++i) {
        synth.addVoice(new SynthVoice());
    }
    synth.addSound(new SynthSound());
    
    
}

WaveSynthAudioProcessor::~WaveSynthAudioProcessor()
{
}

//==============================================================================
const juce::String WaveSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WaveSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WaveSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WaveSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WaveSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WaveSynthAudioProcessor::getNumPrograms()
{
    return 1;   
}

int WaveSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WaveSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WaveSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void WaveSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WaveSynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    synth.setCurrentPlaybackSampleRate(sampleRate);

    reverb.prepareToPlay(sampleRate,samplesPerBlock,getTotalNumOutputChannels());
    drive.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    chorus.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    flanger.prepareToPlay(spec);
    phaser.prepareToPlay(spec);
    delay.prepareToPlay(spec);
    filter.prepareToPlay(sampleRate, getTotalNumOutputChannels());
    lfo.prepare(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }

}

void WaveSynthAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WaveSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WaveSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    oscBufferUI.setSize(buffer.getNumChannels(), buffer.getNumSamples(), false, false, true);
    oscBufferInitial.setSize(buffer.getNumChannels(), buffer.getNumSamples(), false, false, true);
    
    //adsr
    auto& attack = *apvts.getRawParameterValue("ATTACK1");
    auto& decay = *apvts.getRawParameterValue("DECAY1");
    auto& sustain = *apvts.getRawParameterValue("SUSTAIN1");
    auto& release = *apvts.getRawParameterValue("RELEASE1");
    auto& attack2 = *apvts.getRawParameterValue("ATTACK2");
    auto& decay2 = *apvts.getRawParameterValue("DECAY2");
    auto& sustain2 = *apvts.getRawParameterValue("SUSTAIN2");
    auto& release2 = *apvts.getRawParameterValue("RELEASE2");

    //osc
    auto& oscWaveChoice = *apvts.getRawParameterValue("OSC1WAVETYPE");
    auto& oscWaveChoice2 = *apvts.getRawParameterValue("OSC2WAVETYPE");

    auto& oscOct1 = *apvts.getRawParameterValue("OSCTONES1");
    auto& oscSemitone1 = *apvts.getRawParameterValue("OSCSEMITONES1");
    auto& oscOct2 = *apvts.getRawParameterValue("OSCTONES2");
    auto& oscSemitone2 = *apvts.getRawParameterValue("OSCSEMITONES2");
    

    //gain
    auto& volume1 = *apvts.getRawParameterValue("GAIN1");
    auto& volume2 = *apvts.getRawParameterValue("GAIN2");

    //LFO
    auto& lfoWaveform = *apvts.getRawParameterValue("LFO_WAVEFORM");
    auto& lfoTarget = *apvts.getRawParameterValue("LFO_TARGET");
    auto& lfoMix = *apvts.getRawParameterValue("LFO_MIX");
    auto& lfoRatestep = *apvts.getRawParameterValue("LFO_RATE");
    auto& lfoIsEnabled = *apvts.getRawParameterValue("LFO_ENABLED");
    auto& lfoDepth = *apvts.getRawParameterValue("LFO_DEPTH");

    const float freq = lfoRatestep;
    lfo.setWaveform(lfoWaveform);
    lfo.setRateHz(freq);
    float lfoValue;

    float totalOffset1 = oscOct1 * 12.0f + oscSemitone1;
    float totalOffset2 = oscOct2 * 12.0f + oscSemitone2;

    if(lfoIsEnabled)
        lfoValue = lfo.getNextValue() * lfoMix;

    //

    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice *>(synth.getVoice(i))) {

            if (lfoIsEnabled && lfoTarget == 1) {
                float volume1Mod = volume1 * (1 + lfoValue );
                float volume2Mod = volume2 * (1 + lfoValue );
                
                voice->updateGain(volume1Mod, volume2Mod);
            }

            else {
                voice->updateGain(volume1, volume2);
            }
                
            //update adsr
            voice->updateADSR(attack.load(), sustain.load(), decay.load(), release.load(),
                              attack2.load(), sustain2.load(), decay2.load(), release2.load());

            //init osc
            voice->getOscillator().setWaveType(oscWaveChoice);
            voice->getOscillator2().setWaveType(oscWaveChoice2);    

            //frequency offset
            voice->updateOffset(totalOffset1, totalOffset2);
        }
    }
    
    //de aici se adauga efectele si filtrele pentru procesarea semnalului generat de o voce

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    synth.renderNextBlock(buffer,midiMessages,0,buffer.getNumSamples());

    juce::dsp::AudioBlock<float>audioBlock(buffer);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        oscBufferInitial.copyFrom(ch, 0, buffer, ch, 0, buffer.getNumSamples());


    
    //distortion
    auto& driveValue = *apvts.getRawParameterValue("DRIVE");
    auto& driveMix = *apvts.getRawParameterValue("DRIVE_MIX");
    auto& distType = *apvts.getRawParameterValue("DISTTYPE");
    auto& driveEnable = *apvts.getRawParameterValue("DRIVE_ENABLED");

    if (driveEnable) {
        drive.updateDistortion(distType, driveValue);
        drive.processNextBlock(audioBlock);
    }
   
    //reverb init and update
    auto& roomSize = *apvts.getRawParameterValue("ROOMSIZE");
    auto& damp = *apvts.getRawParameterValue("DAMPING");
    auto& wet = *apvts.getRawParameterValue("WETLEVEL");
    auto& reverbEnable = *apvts.getRawParameterValue("REVERB_ENABLED");

    if (reverbEnable) {
        reverb.updateReverb(roomSize.load(), damp.load(), wet.load());
        reverb.getNextAudioBlock(audioBlock);
    }

    //flanger init and update
    auto& flangerRate = *apvts.getRawParameterValue("FLANGER_RATE");
    auto& flangerDepth = *apvts.getRawParameterValue("FLANGER_DEPTH");
    auto& flangerFeedback = *apvts.getRawParameterValue("FLANGER_FEEDBACK");
    auto& flangerMix = *apvts.getRawParameterValue("FLANGER_MIX");
    auto& flangerEnable = *apvts.getRawParameterValue("FLANGER_ENABLED");

    if (flangerEnable) {
        flanger.updateFlanger(flangerRate.load(), flangerDepth.load(), flangerFeedback.load(), flangerMix.load());
        flanger.processNextBlock(audioBlock);
    }

    //Chorus update and init
    auto& rate = *apvts.getRawParameterValue("RATE");
    auto& depth = *apvts.getRawParameterValue("DEPTH");
    auto& delayCenter = *apvts.getRawParameterValue("DELAYCENTER");
    auto& feedBack = *apvts.getRawParameterValue("FEEDBACK");
    auto& mix = *apvts.getRawParameterValue("MIX");
    auto& chorusEnable = *apvts.getRawParameterValue("CHORUS_ENABLED");

    if (chorusEnable) {
        chorus.updateChorus(rate.load(), depth.load(), delayCenter.load(), feedBack.load(), mix.load());
        chorus.processNextBlock(audioBlock);
    }
    

    auto& phaserRate = *apvts.getRawParameterValue("PHASER_RATE");
    auto& phaserDepth = *apvts.getRawParameterValue("PHASER_DEPTH");
    auto& phaserFeedBack = *apvts.getRawParameterValue("PHASER_FEEDBACK");
    auto& phaserMix = *apvts.getRawParameterValue("PHASER_MIX");
    auto& phaserEnable = *apvts.getRawParameterValue("PHASER_ENABLED");

    if (phaserEnable) {
        phaser.updatePhaser(phaserRate.load(), phaserDepth.load(), phaserFeedBack.load(), phaserMix.load());
        phaser.processNextBlock(audioBlock);
    }

    auto& delayDepth = *apvts.getRawParameterValue("DELAY_DEPTH");
    auto& delayFeedback = *apvts.getRawParameterValue("DELAY_FEEDBACK");
    auto& delayMix = *apvts.getRawParameterValue("DELAY_MIX");
    auto& delayIsEnabled = *apvts.getRawParameterValue("DELAY_ENABLED");
   
    if (delayIsEnabled) {
        delay.updateDelay(delayDepth.load(), delayFeedback.load(), delayMix.load());
        delay.processNextBlock(audioBlock);
    }
    
    auto& filterCutoff = *apvts.getRawParameterValue("FILTER_CUTOFF");
    auto& filterReso = *apvts.getRawParameterValue("FILTER_RESONANCE");
    auto& filterMix = *apvts.getRawParameterValue("FILTER_MIX");
    auto& filterType = *apvts.getRawParameterValue("FILTER_TYPE");
    auto& filterIsEnabled = *apvts.getRawParameterValue("FILTER_ENABLED");

    //lfoValue

    if(filterIsEnabled){
        if (lfoIsEnabled && lfoTarget == 0) {
            float modCutoff = filterCutoff + lfoValue * lfoDepth;
            modCutoff = juce::jlimit(20.0f, 18000.0f, modCutoff);
            filter.updateParameters(filterType, modCutoff, filterReso, filterMix);
            filter.processBlock(audioBlock);
        }
        else
        {
            filter.updateParameters(filterType, filterCutoff, filterReso, filterMix);
            filter.processBlock(audioBlock);
        }
        
    }

    //Vizualizare unde oscilator in UI
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        oscBufferUI.copyFrom(ch, 0, buffer, ch, 0, buffer.getNumSamples());
    
   

}

//==============================================================================
bool WaveSynthAudioProcessor::hasEditor() const
{
    return true; 
}

juce::AudioProcessorEditor* WaveSynthAudioProcessor::createEditor()
{
    return new WaveSynthAudioProcessorEditor (*this);
}

//==============================================================================
void WaveSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
   
}

void WaveSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
  
}


juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaveSynthAudioProcessor();
}


juce::AudioProcessorValueTreeState::ParameterLayout WaveSynthAudioProcessor::createParams()
{
    std::vector <std::unique_ptr <juce::RangedAudioParameter> >params;

    // ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK1", "Attack", juce::NormalisableRange<float> { 0.01f, 1.0f, 0.01f }, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY1", "Decay", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN1", "Sustain", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.8f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE1", "Release", juce::NormalisableRange<float> { 0.00f, 1.0f, 0.01f }, 0.01f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK2", "Attack2", juce::NormalisableRange<float> { 0.01f, 1.0f, 0.01f }, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY2", "Decay2", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.3f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN2", "Sustain2", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.8f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE2", "Release2", juce::NormalisableRange<float> { 0.00f, 1.0f, 0.01f }, 0.01f));

    //Osc Selector
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1WAVETYPE", "Osc 1 Wave Type",
    juce::StringArray{"Sine","Saw","Square"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2WAVETYPE", "Osc 2 Wave Type",
    juce::StringArray{ "Sine","Saw","Square" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterBool>("MONO_MODE", "Mono Mode", false)); //Mono Voicing


    //Osc Tones and Semitones
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSCTONES1", "Oscilator Tones", juce::NormalisableRange<float> { -4.0f, 4.0f, 1.0f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSCSEMITONES1", "Oscilator Semitones", juce::NormalisableRange<float> { -12.0f, 12.0f, 1.0f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSCTONES2", "Oscilator Tones", juce::NormalisableRange<float> { -4.0f, 4.0f, 1.0f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSCSEMITONES2", "Oscilator Semitones", juce::NormalisableRange<float> { -12.0f, 12.0f, 1.0f }, 0.0f));
    //Osc Gain
    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN1", "Gain1", juce::NormalisableRange<float> { 0.0f, 0.5f, 0.01f }, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN2", "Gain2", juce::NormalisableRange<float> { 0.0f, 0.5f, 0.01f }, 0.0f));

    //Reverb Params
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ROOMSIZE", "RoomSize", juce::NormalisableRange<float>{0.0f, 1.0f, 0.01f},0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DAMPING", "Damping", juce::NormalisableRange<float>{0.0f, 1.0f, 0.01f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("WETLEVEL", "WetLevel", juce::NormalisableRange<float>{0.0f, 1.0f, 0.01f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("REVERB_ENABLED", "ReverbEnabled", false));

    //Drive Params
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRIVE", "Drive", juce::NormalisableRange<float>{0.0f, 1.0f, 0.01f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("DISTTYPE", "Distortion Type",
    juce::StringArray{ "None","Soft","Hard", "ATAN DIST"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterBool>("DRIVE_ENABLED", "DriveEnabled", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRIVE_MIX", "Drive mix", juce::NormalisableRange<float>{0.0f, 1.0f, 0.01f}, 0.0f));

    //Chorus Params
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RATE", "Rate", juce::NormalisableRange<float>{0.0f, 5.0f, 0.01f}, 0.0f));  
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DEPTH", "Depth", juce::NormalisableRange<float>{0.0f, 1.0f, 0.01f}, 0.0f)); 
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DELAYCENTER", "DelayCenter", juce::NormalisableRange<float>{0.0f, 15.0f, 0.01f}, 0.0f));  
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FEEDBACK", "Feedback", juce::NormalisableRange<float>{0.0f, 1.0f, 0.01f}, 0.0f));  
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MIX", "Mix", juce::NormalisableRange<float>{0.0f, 1.0f, 0.01f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("CHORUS_ENABLED", "ChorusEnabled", false));
    

    //Flanger Params
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FLANGER_RATE", "FlangerRate", juce::NormalisableRange<float> { 0.1f, 1.0f, 0.01f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FLANGER_DEPTH", "FlangerDepth", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FLANGER_FEEDBACK", "FlangerFeedback", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FLANGER_MIX", "FlangerMix", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("FLANGER_ENABLED", "FlangerEnabled", false));
    

    //Phaser Params
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PHASER_RATE", "PhaserRate", juce::NormalisableRange<float> { 0.1f, 5.0f, 0.01f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PHASER_DEPTH", "PhaserDepth", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PHASER_FEEDBACK", "PhaserFeedback", juce::NormalisableRange<float> { -1.0f, 1.0f, 0.01f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PHASER_MIX", "PhaserMix", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("PHASER_ENABLED", "PhaserEnabled", false));

    //Delay Params
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DELAY_DEPTH", "Delay Depth", juce::NormalisableRange<float> { 0.0f, 2000.0f, 0.05f }, 500.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DELAY_FEEDBACK", "Delay Feedback", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DELAY_MIX", "Delay Mix", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("DELAY_ENABLED", "Delay Enabled", false));

    //Filter Params
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FILTER_TYPE", "Filter Type",
        juce::StringArray{ "Low-Pass", "Band-Pass", "High-Pass" }, 2));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER_CUTOFF", "Filter Cutoff Frequency",
        juce::NormalisableRange<float> { 20.0f, 22000.0f, 1.0f, 0.3f }, 1000.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER_RESONANCE", "Filter Resonance",
        juce::NormalisableRange<float> { 0.1f, 1.0f, 0.01f }, 0.5f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER_MIX", "Filter Mix Level",
        juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("FILTER_ENABLED", "Filter Enabled", false));

    //LFO Params
    params.push_back(std::make_unique<juce::AudioParameterChoice>("LFO_WAVEFORM", "LFO Waveform", juce::StringArray{ "Sine", "Square", "Saw", "Triangle" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("LFO_TARGET", "LFO Target", juce::StringArray{ "Filter", "Volume" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LFO_RATE", "LFO Rate", juce::NormalisableRange<float> { 0.0f, 10000.0f, 1.0f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LFO_MIX", "LFO Mix", juce::NormalisableRange<float>{ 0.0f, 1.0f, 0.01f }, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LFO_DEPTH", "LFO Depth", juce::NormalisableRange<float>{ 0.0f, 2000.0f, 1.0f }, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("LFO_ENABLED", "LFO Enabled", false));
    

    return { params.begin(), params.end() };
}


