/*
  ==============================================================================

    LFO.h
    Created: 4 Apr 2025 4:24:55pm
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class LFO
{
public:
    void prepare(double sampleRate);
    void setRateHz(float hz);
    void setWaveform(int waveId);
    float getNextValue();
    void reset();

private:
    double sampleRate = 44100.0;
    float phase = 0.0f;
    float phaseDelta = 0.0f;
    int waveform = 0; // 0 = sine, 1 = square, 2 = saw, 3 = triangle
};
