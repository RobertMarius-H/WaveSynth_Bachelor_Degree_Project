/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 30 Apr 2025 11:05:39am
    Author:  rober

  ==============================================================================
*/

#include "WaveformDisplay.h"

#include "WaveformDisplay.h"

void WaveformDisplay::setBuffer(const juce::AudioBuffer<float>& bufferToDisplay)
{
    bufferCopy = bufferToDisplay; 
    repaint();
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::purple);

    if (bufferCopy.getNumSamples() == 0)
        return;

    auto width = getWidth();
    auto height = getHeight();
    auto* samples = bufferCopy.getReadPointer(0); 

    juce::Path waveform;
    waveform.startNewSubPath(0, height / 2);

    const int numSamples = bufferCopy.getNumSamples();
    const float horizontalScale = static_cast<float>(width) / numSamples;

    for (int i = 0; i < numSamples; ++i)
    {
        float x = i * horizontalScale;
        float y = juce::jmap(samples[i], -1.0f, 1.0f, static_cast<float>(height), 0.0f);
        waveform.lineTo(x, y);
    }

    g.strokePath(waveform, juce::PathStrokeType(1.5f));
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 10.0f, 2.0f);
}
