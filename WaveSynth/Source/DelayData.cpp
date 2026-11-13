/*
  ==============================================================================

    DelayData.cpp
    Created: 12 Mar 2025 12:57:18pm
    Author:  rober

  ==============================================================================
*/

#include "DelayData.h"


DelayData::DelayData() {}
DelayData::~DelayData() {}

void DelayData::setDelayTime(float newTimeMs)
{
    delayTimeMs = juce::jlimit(1.0f, 2000.0f, newTimeMs);
}

void DelayData::setFeedback(float newFeedback)
{
    feedback = juce::jlimit(0.0f, 0.99f, newFeedback); 
}

void DelayData::setMix(float newMix)
{
    mix = juce::jlimit(0.0f, 1.0f, newMix);
}

void DelayData::prepareToPlay(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    delayLine.prepare(spec);

    int maxDelaySamples = static_cast<int>(2.0f * sampleRate); 
    delayLine.setMaximumDelayInSamples(maxDelaySamples);

    reset();
}

void DelayData::processNextBlock(juce::dsp::AudioBlock<float>& block)
{
    const int numSamples = block.getNumSamples();
    const int numChannels = block.getNumChannels();

    float delaySamples = (delayTimeMs / 1000.0f) * sampleRate;

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = block.getChannelPointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float inSample = channelData[sample];
            float delayedSample = delayLine.popSample(channel, delaySamples);

            float outSample = mix * delayedSample + (1.0f - mix) * inSample;
            delayLine.pushSample(channel, inSample + delayedSample * feedback);

            channelData[sample] = outSample;
        }
    }
}

void DelayData::reset()
{
    delayLine.reset();
}

void DelayData::updateDelay(float newDelayTime, float newFeedback, float newMix) {
    setDelayTime(newDelayTime);
    setFeedback(newFeedback);
    setMix(newMix);

}