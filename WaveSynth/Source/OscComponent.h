/*
  ==============================================================================

    OscComponent.h
    Created: 21 Nov 2024 10:30:02pm
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class OscComponent : public juce::Component {
public:
    OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorID, juce::String oscID);
    ~OscComponent();

    void paint(juce::Graphics&)override;
    void resized()override;

private:

    void setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText);

    juce::ComboBox oscSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>oscSelAttachment;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> tonesAttachment, semitonesAttachment;
    juce::Slider toneSlider;
    juce::Slider semitoneSlider;
    juce::Label toneLabel, semitoneLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscComponent)
};