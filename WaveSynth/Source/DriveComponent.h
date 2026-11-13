/*
  ==============================================================================

    DriveComponent.h
    Created: 26 Feb 2025 10:13:56am
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DriveComponent : public juce::Component {
public:
    DriveComponent(juce::AudioProcessorValueTreeState& apvts, juce::String distType);
    ~DriveComponent() {};
    void paint(juce::Graphics&)override;
    void resized() override;

private:
    juce::ComboBox distSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>distSelAttachment;

    juce::Label driveLabel;
    juce::Label mixLabel;

    void setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText);

    juce::Slider driveSlider;
    juce::Slider mixSlider;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> driveAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment;
};