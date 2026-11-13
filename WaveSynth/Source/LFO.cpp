/*
  ==============================================================================

    LFO.cpp
    Created: 4 Apr 2025 4:24:55pm
    Author:  rober

  ==============================================================================
*/

#include "LFO.h"

#include "LFO.h"

void LFO::prepare(double sr) {
    sampleRate = sr;
}

void LFO::setRateHz(float hz) {
    phaseDelta = hz / sampleRate;
}

void LFO::setWaveform(int waveId) {
    waveform = waveId;
}

void LFO::reset() {
    phase = 0.0f;
}

float LFO::getNextValue() {
    float value = 0.0f;
    float t = phase;

    switch (waveform) {
    case 0: // Sine
        value = std::sin(juce::MathConstants<float>::twoPi * t);
        
        break;
    case 1: // Square
        value = std::sin(juce::MathConstants<float>::twoPi * t) >= 0.0f ? 1.0f : -1.0f;
        break;
    case 2: // Saw
        value = 2.0f * t - 1.0f;
        break;
    case 3: // Triangle
        value = 2.0f * std::abs(2.0f * t - 1.0f) - 1.0f;
        break;
    }

    phase += phaseDelta;
    if (phase >= 1.0f)
        phase -= 1.0f;

    return value;
}
