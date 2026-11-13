/*
  ==============================================================================

    GainData.cpp
    Created: 3 Mar 2025 5:07:11pm
    Author:  rober

  ==============================================================================
*/

#include "GainData.h"

void GainData::prepareToPlay(juce::dsp::ProcessSpec& spec) {
    prepare(spec);
}

void GainData::updateVolume(static float volume) {
    setGainLinear(volume);
}

void GainData::processNextBlock(juce::dsp::AudioBlock<float>& block) {
    process(juce::dsp::ProcessContextReplacing<float>(block));
}