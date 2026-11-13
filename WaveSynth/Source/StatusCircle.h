/*
  ==============================================================================

    StatusCircle.h
    Created: 30 Apr 2025 10:48:45am
    Author:  rober

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class StatusCircle : public juce::Component
{
public:
    void setActive(bool shouldBeActive);
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    bool isActive = false;
};