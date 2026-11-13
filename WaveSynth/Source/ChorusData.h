/*
  ==============================================================================

    ChorusData.h
    Created: 4 Mar 2025 5:14:21pm
    Author:  rober

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ChorusData : public juce::dsp::Chorus<float>{
private:
    juce::dsp::Chorus<float> chorus; //avem nevoie de un obiect sa putem apela functiile pentru a da update la chorus

public:
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void processNextBlock(juce::dsp::AudioBlock<float>& audioBlock);
    void updateChorus(float rate, float depth, float delayCenter, float feedBack, float mix);
    bool isEnabled(const int choice);
};