/*
  ==============================================================================

    FlangerComponent.h
    Created: 5 Mar 2025 6:13:40pm
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FlangerComponent : public juce::Component {

public:

    FlangerComponent(juce::AudioProcessorValueTreeState& apvts);
    ~FlangerComponent();
    void paint(juce::Graphics&)override;
    void resized() override;

private:

    juce::Label rateLabel, depthLabel, feedbackLabel, wetLabel;

    void setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText);

    juce::Slider rateSlider;
    juce::Slider depthSlider;
    juce::Slider feedbackSlider;
    juce::Slider wetSlider;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    //declarare action listeners
    std::unique_ptr<SliderAttachment> rateAttachment;
    std::unique_ptr<SliderAttachment> depthAttachment;
    std::unique_ptr<SliderAttachment> feedbackAttachment;
    std::unique_ptr<SliderAttachment> wetAttachment;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FlangerComponent);
};