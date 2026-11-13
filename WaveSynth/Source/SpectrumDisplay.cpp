/*
  ==============================================================================

    SpectrumDisplay.cpp
    Created: 30 Apr 2025 11:59:22am
    Author:  rober

  ==============================================================================
*/

#include "SpectrumDisplay.h"

SpectrumDisplay::SpectrumDisplay() {}

void SpectrumDisplay::setBuffer(const juce::AudioBuffer<float>& bufferToAnalyse)
{
    const auto* channelData = bufferToAnalyse.getReadPointer(0);
    const int numSamples = std::min(bufferToAnalyse.getNumSamples(), static_cast<int>(fftSize));


    std::fill(fftInput.begin(), fftInput.end(), 0.0f);

    if (channelData != nullptr)
        std::copy_n(channelData, numSamples, fftInput.begin());

    fft.performRealOnlyForwardTransform(fftInput.data());

    for (int i = 0; i < fftSize / 2; ++i)
    {
        const auto re = fftInput[i * 2];
        const auto im = fftInput[i * 2 + 1];
        fftOutput[i] = std::sqrt(re * re + im * im);
    }

    updateSpectrumPath();
    repaint();
}


void SpectrumDisplay::updateSpectrumPath()
{
    const int width = getWidth();
    const int height = getHeight();

    const float topPadding = 10.0f;
    const float bottomPadding = 10.0f;
    const float verticalOffset = 30.0f;
    const float horizontalOffset = 0.0f;

    spectrumPath.clear();
    const int numBins = fftSize / 2;

    for (int i = 1; i < numBins; i += 2)
    {
        float magnitude = fftOutput[i];
        float dB = juce::Decibels::gainToDecibels(magnitude);
        dB = juce::jlimit(-100.0f, 0.0f, dB);

        float x = std::log10((float)i + 1) / std::log10((float)numBins) * (float)width + horizontalOffset;

        float y = juce::jmap(
            dB,
            -100.0f, 0.0f,
            height - bottomPadding + verticalOffset,
            topPadding + verticalOffset
        );

        if (i == 1)
            spectrumPath.startNewSubPath(x, y);
        else
            spectrumPath.lineTo(x, y);
    }
}




void SpectrumDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::purple);
    g.strokePath(spectrumPath, juce::PathStrokeType(1.5f));
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 10.0f, 2.0f);
}
