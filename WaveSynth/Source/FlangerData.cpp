/*
  ==============================================================================

    FlangerData.cpp
    Created: 5 Mar 2025 6:13:31pm
    Author:  rober

  ==============================================================================
*/

#include "FlangerData.h"

FlangerData::FlangerData()
    : lfo([](float x) { return std::sin(x); }) // LFO sinusoidal
{
}
/*
void FlangerData::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    lfo.prepare(spec);
}
*/

void FlangerData::prepareToPlay(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    auto samplesPerBlock = spec.maximumBlockSize;
    auto outputChannels = spec.numChannels;

    maxDelaySamples = static_cast<int>(maxDelayMs * sampleRate / 1000.0f);
    jassert(maxDelaySamples > 0);  

    delayLine.prepare(spec);
    delayLine.setMaximumDelayInSamples(static_cast<size_t>(maxDelaySamples));
    delayLine.reset();

    lfo.prepare({ sampleRate, static_cast<juce::uint32>(samplesPerBlock), static_cast<juce::uint32>(outputChannels) });
    lfo.setFrequency(rateHz);
}

void FlangerData::processNextBlock(juce::dsp::AudioBlock<float>& block)
{
    const auto numChannels = block.getNumChannels();
    const auto numSamples = block.getNumSamples();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            // Normalize LFO to [0,1]
            const float lfoValue = (lfo.processSample(0.0f) + 1.0f) * 0.5f;
            
            // Compute delay time (bounded)
            const float delayTimeMs = juce::jlimit(0.0f, maxDelayMs, baseDelayMs + depthMs * lfoValue);
            const float delayTimeSamples = juce::jlimit(0.0f, static_cast<float>(maxDelaySamples - 1.0f), delayTimeMs * sampleRate / 1000.0f);

            // Read delayed sample safely
            const float delayedSample = delayLine.popSample(channel, delayTimeSamples, true);

            // Process input signal
            const float inputSample = channelData[sample];
            const float outputSample = inputSample * (1.0f - mix) + delayedSample * mix;

            // Write feedback into delay line
            delayLine.pushSample(channel, inputSample + feedbackGain * delayedSample);
            // Store processed sample
            channelData[sample] = outputSample;
        }
    }
}


void FlangerData::setRate(float newRate)
{
    rateHz = newRate;
    lfo.setFrequency(rateHz);
}

void FlangerData::setDepth(float newDepth)
{
    depthMs = juce::jlimit(0.0f, 1.0f, newDepth);
}

void FlangerData::setFeedback(float newFeedback)
{
    feedbackGain = juce::jlimit(0.0f, 1.0f, newFeedback);
}

void FlangerData::setMix(float newMix)
{
    mix = juce::jlimit(0.0f, 1.0f, newMix);
}

void FlangerData::updateFlanger(float rate, float depth, float feedback, float mix) {
    setRate(rate);
    setDepth(depth);
    setFeedback(feedback);
    setMix(mix);
}

bool FlangerData ::isEnabled() {
    return true;
}