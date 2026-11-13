/*
  ==============================================================================

    PhaserData.cpp
    Created: 12 Mar 2025 12:57:02pm
    Author:  rober

  ==============================================================================
*/

#include "PhaserData.h"

PhaserData::PhaserData() {}
PhaserData::~PhaserData() {}


void PhaserData::setRate(float newRate) { rate = newRate; }
void PhaserData::setDepth(float newDepth) { depth = newDepth; }
void PhaserData::setFeedback(float newFeedback) { feedback = newFeedback; }
void PhaserData::setMix(float newMix) { mix = newMix; }

void PhaserData::prepareToPlay(const juce::dsp::ProcessSpec& spec) {
    sampleRate = spec.sampleRate;
    for (auto& filter : allpassFilters)
        filter.prepare(spec);

    reset();
}

void PhaserData::processNextBlock(juce::dsp::AudioBlock<float>& block)
{
    const int numSamples = block.getNumSamples();
    const int numChannels = block.getNumChannels();

    if (feedback == -1.0f)
        reset();

    for (int sample = 0; sample < numSamples; ++sample)
    {
        float lfoValue = 0.5f * (1.0f + std::sin(lfoPhase));
        float allpassFreq = 300.0f + 2000.0f * (depth * lfoValue);
        lfoPhase += juce::MathConstants<float>::twoPi * (rate / sampleRate);
        if (lfoPhase > juce::MathConstants<float>::twoPi)
            lfoPhase -= juce::MathConstants<float>::twoPi;

        for (auto& filter : allpassFilters)
            filter.coefficients = juce::dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, allpassFreq);

        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* channelData = block.getChannelPointer(channel);
            float inSample = channelData[sample] + feedbackSample * feedback;

            for (auto& filter : allpassFilters)
                inSample = filter.processSample(inSample);

            feedbackSample = inSample;
            channelData[sample] = mix * inSample + (1.0f - mix) * channelData[sample];
        }
    }
}


void PhaserData::updatePhaser(float rate, float depth, float feedback, float mix) {
    setRate(rate);
    setDepth(depth);
    setFeedback(feedback);
    setMix(mix);
}

void PhaserData::reset()
{
    for (auto& filter : allpassFilters)
        filter.reset();
    feedbackSample = 0.0f;
    lfoPhase = 0.0f;
}
