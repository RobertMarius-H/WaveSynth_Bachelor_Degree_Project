/*
  ==============================================================================

    ReverbData.cpp
    Created: 10 Feb 2025 10:59:07am
    Author:  rober

  ==============================================================================
*/

#include "ReverbData.h"

void ReverbData::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    prepare(spec);
}


void ReverbData::updateReverb(const float roomSize, const float damp, const float wet) {
    reverbParams.roomSize = roomSize;
    reverbParams.damping = damp;
    reverbParams.wetLevel = wet;
    setParameters(reverbParams);
}

void ReverbData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block) {
    process(juce::dsp::ProcessContextReplacing<float>(block));
}
