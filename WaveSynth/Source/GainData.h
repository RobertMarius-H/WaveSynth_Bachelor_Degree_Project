/*
  ==============================================================================

    GainData.h
    Created: 3 Mar 2025 5:07:11pm
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class GainData : public juce::dsp::Gain<float> {
public:
    void prepareToPlay(juce::dsp::ProcessSpec& spec);
    void updateVolume(static float vol);
    void processNextBlock(juce::dsp::AudioBlock<float>& block);
private:

};

