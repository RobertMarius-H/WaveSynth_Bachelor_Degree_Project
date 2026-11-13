/*
  ==============================================================================

    GainComponent.h
    Created: 3 Mar 2025 2:27:34pm
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class GainComponent : public juce::Component {
public:
    GainComponent(juce::AudioProcessorValueTreeState& apvts, const juce::String& prefix);
    ~GainComponent();

    void paint(juce::Graphics&) override;   
    void resized() override;


private:
    void setupSilderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText);

    juce::Slider volumeSlider;
    juce::Label volumeLabel;


    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> volumeAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComponent)
};