/*
  ==============================================================================

    AdsrData.h
    Created: 19 Nov 2024 6:02:25pm
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class AdsrData : public juce::ADSR {
public:
    void updateADSR(const float attack, const float decay, const float sustain, const float release);

private:

    juce::ADSR::Parameters adsrParams;
};