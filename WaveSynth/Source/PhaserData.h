/*
  ==============================================================================

    PhaserData.h
    Created: 12 Mar 2025 12:57:02pm
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class PhaserData {
public:
    PhaserData();
    ~PhaserData();
    void setRate(float newRate);
    void setDepth(float newDepth);
    void setFeedback(float newFeedback);
    void setMix(float newMix);
    void prepareToPlay(const juce::dsp::ProcessSpec& spec);
    void processNextBlock(juce::dsp::AudioBlock<float>& buffer);
    void updatePhaser(float rate, float depth, float feedback, float mix);
    void reset();

private:
    float sampleRate;
    float rate = 0.5f;
    float depth = 0.5f;
    float feedback = 0.0f;
    float mix = 0.5f;

    juce::dsp::IIR::Filter<float> allpassFilters[6];
    float lfoPhase = 0.0f;
    float feedbackSample = 0.0f;
};