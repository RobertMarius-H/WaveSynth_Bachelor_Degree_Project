/*
  ==============================================================================

    DriveData.cpp
    Created: 26 Feb 2025 10:13:48am
    Author:  rober

  ==============================================================================
*/

#include "DriveData.h"

void DriveData::processNextBlock(juce::dsp::AudioBlock<float>& block) {
    distortion.process(juce::dsp::ProcessContextReplacing<float>(block));
}

void DriveData::updateDistortion(const int distortionType, float driveAmount) {
    

    distortion.functionToUse = [](float x) { return x; };

    switch (distortionType) {
    case 1: // Soft Clipping
        distortion.functionToUse = [driveAmount](float x) {
            return juce::jlimit(-0.8f * driveAmount , 0.8f * driveAmount , x);
            };
        break;

    case 2: // Hard Clipping
        distortion.functionToUse = [driveAmount](float x) {
            return juce::jlimit(-0.3f * driveAmount, 0.3f * driveAmount, x);
            };
        break;

    case 3: // Arctangent
        distortion.functionToUse = [driveAmount](float x) {
            return (2.0f / juce::MathConstants<float>::pi) * std::atan(x * driveAmount * 10.0f);
            };
        break;

    default:
        distortion.functionToUse = [](float x) { return x; };
        break;
    }
}


void DriveData::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    distortion.prepare(spec);
}