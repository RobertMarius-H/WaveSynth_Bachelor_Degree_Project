/*
  ==============================================================================

    DelayData.h
    Created: 12 Mar 2025 12:57:18pm
    Author:  rober

  ==============================================================================
*/

#pragma once
#pragma once
#include <JuceHeader.h>

class DelayData
{
public:
    DelayData();
    ~DelayData();

    void setDelayTime(float newTimeMs);
    void setFeedback(float newFeedback);
    void setMix(float newMix);
    void updateDelay(float newDelayTime, float newFeedback, float newMix);

    void prepareToPlay(const juce::dsp::ProcessSpec& spec);
    void processNextBlock(juce::dsp::AudioBlock<float>& block);
    void reset();

private:
    float sampleRate = 44100.0f;
    float delayTimeMs = 500.0f; 
    float feedback = 0.5f;
    float mix = 0.5f;

    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;
};
