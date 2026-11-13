/*
  ==============================================================================

    StatusCircle.cpp
    Created: 30 Apr 2025 10:46:46am
    Author:  rober

  ==============================================================================
*/

#include "StatusCircle.h"


    void StatusCircle::setActive(bool shouldBeActive)
    {
        isActive = shouldBeActive;
        repaint();
    }

void StatusCircle::paint(juce::Graphics& g)
    {
        auto bounds = getLocalBounds().toFloat().reduced(1.0f);
        g.setColour(isActive ? juce::Colours::green : juce::Colours::red);
        g.fillEllipse(bounds);
    }

void StatusCircle::resized(){

}

