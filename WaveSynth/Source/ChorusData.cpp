/*
  ==============================================================================

    ChorusData.cpp
    Created: 4 Mar 2025 5:14:21pm
    Author:  rober

  ==============================================================================
*/

#include "ChorusData.h"

void ChorusData::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    chorus.prepare(spec);
}

void ChorusData::processNextBlock(juce::dsp::AudioBlock<float>& audioBlock) {
    chorus.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

void ChorusData::updateChorus(float rate, float depth, float delayCenter, float feedBack, float mix) {
    chorus.setRate(rate);
    chorus.setDepth(depth);
    chorus.setCentreDelay(delayCenter);
    chorus.setFeedback(feedBack);
    chorus.setMix(mix);
}

bool ChorusData::isEnabled(const int choice){
    switch(choice){
    case 0:
        return false;
        break;
    case 1:
        return true;
        break;
    default:
        jassertfalse;
        break;
    }
    
}

