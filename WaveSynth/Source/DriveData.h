/*
  ==============================================================================

    DriveData.h
    Created: 26 Feb 2025 10:13:48am
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DriveData{
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void processNextBlock(juce::dsp::AudioBlock<float>& block);
    void updateDistortion(const int distortionType,float driveAmount);
private:
    juce::dsp::WaveShaper<float, std::function<float(float)>> distortion;
};
