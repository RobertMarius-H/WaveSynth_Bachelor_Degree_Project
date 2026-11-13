/*
  ==============================================================================

    PhaserComponent.h
    Created: 12 Mar 2025 12:57:11pm
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class PhaserComponent : public juce::Component {

public:

    PhaserComponent(juce::AudioProcessorValueTreeState& apvts);
    ~PhaserComponent();
    void paint(juce::Graphics&)override;
    void resized() override;

private:

    juce::Label rateLabel, depthLabel, feedBackLabel, mixLabel;

    void setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText);

    juce::Slider rateSlider;
    juce::Slider depthSlider;
    juce::Slider feedBackSlider;
    juce::Slider mixSlider;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    //declarare action listeners
    std::unique_ptr<SliderAttachment> rateAttachment;
    std::unique_ptr<SliderAttachment> depthAttachment;
    std::unique_ptr<SliderAttachment> feedBackAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PhaserComponent);
};