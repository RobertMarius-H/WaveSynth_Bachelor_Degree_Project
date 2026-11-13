/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ReverbData.h"
#include "DriveData.h"
#include "GainData.h"
#include "ChorusData.h"
#include "FlangerData.h"
#include "PhaserData.h"
#include "DelayData.h"
#include "FilterData.h"
#include "LFO.h"

//==============================================================================
/**
*/
class WaveSynthAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    WaveSynthAudioProcessor();
    ~WaveSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;


    juce::AudioProcessorValueTreeState apvts;  
    juce::MidiKeyboardState keyboardState;
    FilterData filter;
    juce::AudioBuffer<float> oscBufferUI;
    juce::AudioBuffer<float> oscBufferInitial;

private:

    //efecte
    ReverbData reverb;
    DriveData drive;
    ChorusData chorus;
    FlangerData flanger;
    PhaserData phaser;
    DelayData delay;
    
    LFO lfo;

    
    juce::dsp::ProcessSpec spec;
    juce::Synthesiser synth;
    static const int numVoices = 16;
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveSynthAudioProcessor)

};
