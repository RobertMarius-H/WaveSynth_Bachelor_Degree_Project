/*
  ==============================================================================

    ChorusComponent.h
    Created: 4 Mar 2025 5:14:28pm
    Author:  rober

  ==============================================================================
*/


#pragma once
#include <JuceHeader.h>

class ChorusComponent : public juce::Component {

public:

    ChorusComponent(juce::AudioProcessorValueTreeState& apvts);
    ~ChorusComponent();
    void paint(juce::Graphics&)override;
    void resized() override;

private:

    juce::Label rateLabel, depthLabel, delayCenterLabel,feedBackLabel,mixLabel;


    void setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText);

    juce::Slider rateSlider;
    juce::Slider depthSlider;
    juce::Slider delayCenterSlider;
    juce::Slider feedBackSlider;
    juce::Slider mixSlider;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    //declarare action listeners
    std::unique_ptr<SliderAttachment> rateAttachment;
    std::unique_ptr<SliderAttachment> depthAttachment;
    std::unique_ptr<SliderAttachment> delayCenterAttachment;
    std::unique_ptr<SliderAttachment> feedBackAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment;

    juce::ToggleButton enable;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChorusComponent);
};