/*
  ==============================================================================

    DelayComponent.h
    Created: 12 Mar 2025 12:57:25pm
    Author:  rober

  ==============================================================================
*/

#pragma once
#pragma once
#include <JuceHeader.h>
class DelayComponent : public juce::Component {

public:

    DelayComponent(juce::AudioProcessorValueTreeState& apvts);
    ~DelayComponent();
    void paint(juce::Graphics&)override;
    void resized() override;

private:

    juce::Label delayLabel, feedbackLabel, mixLabel;

    void setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText);

    juce::Slider delaySlider;
    juce::Slider feedbackSlider;
    juce::Slider mixSlider;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    //declarare action listeners
   
    std::unique_ptr<SliderAttachment> delayAttachment;
    std::unique_ptr<SliderAttachment> feedbackAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayComponent);
};