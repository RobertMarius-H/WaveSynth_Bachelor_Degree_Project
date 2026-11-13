/*
  ==============================================================================

    ReverbData.h
    Created: 10 Feb 2025 10:59:07am
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class ReverbData : public juce::dsp::Reverb {
private:
    juce::dsp::Reverb::Parameters reverbParams;
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void updateReverb(const float roomSize, const float damp, const float wet);
    void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);
};
