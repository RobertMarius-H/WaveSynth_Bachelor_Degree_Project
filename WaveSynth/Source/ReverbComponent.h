/*
  ==============================================================================

    ReverbComponent.h
    Created: 10 Feb 2025 10:59:20am
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ReverbComponent : public juce::Component {

public:

    ReverbComponent(juce::AudioProcessorValueTreeState& apvts);
    ~ReverbComponent();
    void paint(juce::Graphics&)override;
    void resized() override;

private:

    juce::Label roomLabel, dampLabel, wetLabel;

    void setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText);

    juce::Slider roomSizeSlider;
    juce::Slider dampSlider;
    juce::Slider wetSlider;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    //declarare action listeners
    std::unique_ptr<SliderAttachment> roomSizeAttachment;
    std::unique_ptr<SliderAttachment> dampAttachment;
    std::unique_ptr<SliderAttachment> wetAttachment;




    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbComponent);
};