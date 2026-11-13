/*
  ==============================================================================

    GainComponent.cpp
    Created: 3 Mar 2025 2:27:34pm
    Author:  rober

  ==============================================================================
*/

#include "GainComponent.h"

GainComponent::GainComponent(juce::AudioProcessorValueTreeState& apvts, const juce::String& prefix) {

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    volumeAttachment = std::make_unique<SliderAttachment>(apvts, "GAIN" + prefix, volumeSlider);
    setupSilderWithLabel(volumeSlider, volumeLabel, "Volume" + prefix);
}
GainComponent::~GainComponent() {

}


void GainComponent::setupSilderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText) {
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::blueviolet);
    slider.setColour(juce::Slider::thumbColourId, juce::Colours::violet);

    addAndMakeVisible(label);
    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colours::white);
}

void GainComponent::resized() {
    const auto startY = 0;
    const auto sliderWidth = 80;
    const auto sliderHeight = 80;
    const auto labelYOffset = 70;
    const auto labelHeight = 20;

    volumeSlider.setBounds(10, startY + 5,75, 75);
    volumeLabel.setBounds(volumeSlider.getX(), volumeSlider.getY() + 40, volumeSlider.getWidth(), volumeSlider.getHeight());
  
}


void GainComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::purple);
    g.setFont(juce::FontOptions(15.0f));
    auto bounds = getLocalBounds();
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 10.0f, 4.0f);
}