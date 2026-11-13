/*
  ==============================================================================

    SynthSound.h
    Created: 29 Oct 2024 10:51:00pm
    Author:  rober

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class SynthSound : public juce::SynthesiserSound {
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};