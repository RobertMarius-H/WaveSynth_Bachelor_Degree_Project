#include "FilterData.h"

FilterData::FilterData() : sampleRateHz(44100.0f), numChannels(2), filterType(1),
cutoffFrequencyHz(1000.0f), resonance(0.5f), mix(1.0f),
filterCoefficient(0.0f) {}

FilterData::~FilterData() {}

void FilterData::prepareToPlay(double sampleRate, int numChannels) {
    sampleRateHz = static_cast<float>(sampleRate);
    this->numChannels = numChannels;
    updateCoefficients();
}

void FilterData::updateParameters(int type, float cutoff, float res, float mixAmount) {
    filterType = type;
    cutoffFrequencyHz = cutoff;
    resonance = res;
    mix = mixAmount;
    updateCoefficients();
}

void FilterData::updateCoefficients() {
    float wc = 2* juce::MathConstants<float>::pi  * cutoffFrequencyHz / sampleRateHz;
    filterCoefficient = wc / (wc + 1.0f);
    nonLinearGain = 1.5f * (1.0f / (1.0f + resonance * 2.0f));
}

float FilterData::processSample(float inputSample, int channel) {
    float feedbackInput = inputSample - resonance * stage4Output[channel];

    stage1Output[channel] += filterCoefficient * (feedbackInput - stage1Output[channel]);
    stage2Output[channel] += filterCoefficient * (stage1Output[channel] - stage2Output[channel]);
    stage3Output[channel] += filterCoefficient * (stage2Output[channel] - stage3Output[channel]);
    stage4Output[channel] += filterCoefficient * (stage3Output[channel] - stage4Output[channel]);

    float lowPassOut = stage4Output[channel];
    float highPassOut = inputSample - lowPassOut;
    float bandPassOut = highPassOut - lowPassOut;

    float filteredSample = 0.0f;
    if (filterType == 0)       filteredSample = lowPassOut;
    else if (filterType == 1)  filteredSample = highPassOut;
    else if (filterType == 2)  filteredSample = bandPassOut;

    return (filteredSample * nonLinearGain) + (inputSample * (1.0f - mix));
}

void FilterData::processBlock(juce::dsp::AudioBlock<float>& block) {
    for (size_t channel = 0; channel < block.getNumChannels(); ++channel) {
        for (size_t sample = 0; sample < block.getNumSamples(); ++sample) {
            block.getChannelPointer(channel)[sample] = processSample(block.getChannelPointer(channel)[sample], channel);
        }
    }
}

