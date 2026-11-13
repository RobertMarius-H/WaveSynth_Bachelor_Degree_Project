/*
  ==============================================================================

    OscData.h
    Created: 7 Dec 2024 12:36:35pm
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class OscData : public juce::dsp::Oscillator<float> {

public:

    void prepareToPlay(juce::dsp::ProcessSpec& spec);
    void setWaveType(const int choice);
    void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);
    void setOscFrequency(const int midiNoteNumber);
    void setFrequencyOffset(float mewOffset);

private:
    float baseFrequency = 440.0f;
    float offset;
    
};
