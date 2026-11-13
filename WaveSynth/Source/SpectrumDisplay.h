/*
  ==============================================================================

    SpectrumDisplay.h
    Created: 30 Apr 2025 11:59:22am
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SpectrumDisplay : public juce::Component
{
public:
    SpectrumDisplay();
    void setBuffer(const juce::AudioBuffer<float>& bufferToAnalyse);
    void paint(juce::Graphics& g) override;

private:
    enum { fftOrder = 11, fftSize = 1 << fftOrder }; // 2048 samples
    juce::dsp::FFT fft{ fftOrder };

    std::array<float, fftSize> fftInput{};
    std::array<float, fftSize * 2> fftOutput{};
    juce::Path spectrumPath;
    void updateSpectrumPath();
};
