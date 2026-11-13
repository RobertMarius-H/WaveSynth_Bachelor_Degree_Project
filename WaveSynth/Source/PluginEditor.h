/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "AdsrComponent.h"
#include "OscComponent.h"
#include "ReverbComponent.h"
#include "DriveComponent.h"
#include "GainComponent.h"
#include "ChorusComponent.h"
#include "FlangerComponent.h"
#include "PhaserComponent.h"
#include "DelayComponent.h"
#include "FilterComponent.h"
#include "LFOComponent.h"
#include "StatusCircle.h"
#include "WaveformDisplay.h"
#include "SpectrumDisplay.h"
//==============================================================================
class WaveSynthAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    WaveSynthAudioProcessorEditor (WaveSynthAudioProcessor&);
    ~WaveSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
private:

    WaveSynthAudioProcessor& audioProcessor;
    AdsrComponent adsr;
    AdsrComponent adsr2;

    OscComponent osc;
    OscComponent osc2;

    ReverbComponent reverb;
    DriveComponent drive;
    ChorusComponent chorus;
    FlangerComponent flanger;
    PhaserComponent phaser;
    DelayComponent delay;

    GainComponent gain1;
    GainComponent gain2;

    FilterComponent filter;
    LFOComponent lfo;

    juce::ToggleButton delayButton;
    juce::ToggleButton reverbButton;
    juce::ToggleButton chorusButton;
    juce::ToggleButton driveButton;
    juce::ToggleButton phaserButton;
    juce::ToggleButton flangerButton;
    juce::ToggleButton filterButton;
    juce::ToggleButton lfoButton;

    
    std::unique_ptr<juce::ButtonParameterAttachment> delayEnAtt;
    std::unique_ptr<juce::ButtonParameterAttachment> reverbEnAtt;
    std::unique_ptr<juce::ButtonParameterAttachment> chorusEnAtt;
    std::unique_ptr<juce::ButtonParameterAttachment> driveEnAtt;
    std::unique_ptr<juce::ButtonParameterAttachment> phaserEnAtt;
    std::unique_ptr<juce::ButtonParameterAttachment> flangerEnAtt;
    std::unique_ptr<juce::ButtonParameterAttachment> filterEnAtt;
    std::unique_ptr<juce::ButtonParameterAttachment> lfoEnAtt; 

    StatusCircle reverbCircle;
    StatusCircle chorusCircle;
    StatusCircle phaserCircle;
    StatusCircle flangerCircle;
    StatusCircle driveCircle;
    StatusCircle delayCircle;
    StatusCircle lfoCircle;
    StatusCircle filterCircle;

    juce::MidiKeyboardComponent keyboardComponent;
    WaveformDisplay waveformDisplay, waveformInitial;
    SpectrumDisplay spectrumDisplay; 


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveSynthAudioProcessorEditor)
};
