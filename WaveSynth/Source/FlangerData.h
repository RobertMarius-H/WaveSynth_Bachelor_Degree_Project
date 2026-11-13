/*
  ==============================================================================

    FlangerData.h
    Created: 5 Mar 2025 6:13:31pm
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FlangerData 

{
public:
    FlangerData();
    ~FlangerData() = default;

    void prepareToPlay(const juce::dsp::ProcessSpec& spec);
    void processNextBlock(juce::dsp::AudioBlock<float>& block);

    void setRate(float newRate);
    void setDepth(float newDepth);
    void setFeedback(float newFeedback);
    void setMix(float newMix);
    void updateFlanger(float rate, float depth, float feedback, float mix);
    bool isEnabled();

private:
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;
    juce::dsp::Oscillator<float> lfo;

    float sampleRate;
    float rateHz = 0.25f;
    float depthMs = 5.0f; // Default 5ms
    float feedbackGain = 0.3f;
    float mix = 0.5f;

    float baseDelayMs = 1.0f;
    float maxDelayMs = 10.0f; // Max 10ms
    int maxDelaySamples = 0;
};