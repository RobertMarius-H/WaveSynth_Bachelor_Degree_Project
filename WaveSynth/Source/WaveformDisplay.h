/*
  ==============================================================================

    WaveformDisplay.h
    Created: 30 Apr 2025 11:05:39am
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class WaveformDisplay : public juce::Component
{
public:
    void setBuffer(const juce::AudioBuffer<float>& bufferToDisplay);

    void paint(juce::Graphics& g) override;
    void resized() override {}

private:
    juce::AudioBuffer<float> bufferCopy;
};
