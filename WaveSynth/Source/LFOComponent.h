/*
  ==============================================================================

    LFOComponent.h
    Created: 4 Apr 2025 4:28:00pm
    Author:  rober

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class LFOComponent : public juce::Component
{
public:
    LFOComponent(juce::AudioProcessorValueTreeState& apvts);
    ~LFOComponent() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText);

    juce::Slider mixSlider,rateSlider,depthSlider;
    juce::ComboBox waveformSelector, targetSelector;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveformAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> targetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> depthAttachment;

    juce::Label mixLabel;
    juce::Label waveformLabel;
    juce::Label targetLabel;
    juce::Label rateLabel;
    juce::Label depthLabel;
};

