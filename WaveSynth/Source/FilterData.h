#pragma once

#include <JuceHeader.h>

class FilterData {
public:
    FilterData();
    ~FilterData();

    void prepareToPlay(double sampleRate, int numChannels);
    void updateParameters(int type, float cutoff, float resonance, float mix);
    void processBlock(juce::dsp::AudioBlock<float>& block);

private:
    float sampleRateHz;
    int numChannels;

    int filterType;
    float cutoffFrequencyHz;
    float resonance;
    float mix;

    float stage1Output[2] = { 0.0f, 0.0f };
    float stage2Output[2] = { 0.0f, 0.0f };
    float stage3Output[2] = { 0.0f, 0.0f };
    float stage4Output[2] = { 0.0f, 0.0f };
    float highPassState[2] = { 0.0f, 0.0f };
    float bandPassState[2] = { 0.0f, 0.0f };
    
    float nonLinearGain;
    float filterCoefficient;

    void updateCoefficients();
    float processSample(float inputSample, int channel);
};
