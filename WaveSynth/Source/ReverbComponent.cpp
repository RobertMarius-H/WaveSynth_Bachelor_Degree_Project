/*
  ==============================================================================

    ReverbComponent.cpp
    Created: 10 Feb 2025 10:59:20am
    Author:  rober

  ==============================================================================
*/

#include "ReverbComponent.h"

ReverbComponent::ReverbComponent(juce::AudioProcessorValueTreeState& apvts) {
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    roomSizeAttachment = std::make_unique<SliderAttachment>(apvts, "ROOMSIZE", roomSizeSlider);
    wetAttachment = std::make_unique<SliderAttachment>(apvts, "WETLEVEL", wetSlider);
    dampAttachment = std::make_unique<SliderAttachment>(apvts, "DAMPING", dampSlider);

    setupSliderWithLabel(roomSizeSlider, roomLabel, "Room Size");
    setupSliderWithLabel(wetSlider, wetLabel, "Wet Level");
    setupSliderWithLabel(dampSlider, dampLabel, "Damping");

}

ReverbComponent::~ReverbComponent() {

}

void ReverbComponent::setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText) {
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::blueviolet);
    slider.setColour(juce::Slider::thumbColourId, juce::Colours::violet);

    addAndMakeVisible(label);
    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colours::white);
}


void ReverbComponent::resized() {

    const auto startY = 0;
    const auto sliderWidth = 70;
    const auto sliderHeight = 70;
    const auto labelYOffset = 70;
    const auto labelHeight = 20;

    roomSizeSlider.setBounds(0, 0, sliderWidth, sliderHeight);
    dampSlider.setBounds(roomSizeSlider.getRight(), 0, sliderWidth, sliderHeight);
    wetSlider.setBounds(dampSlider.getRight(), 0, sliderWidth, sliderHeight);

    roomLabel.setBounds(roomSizeSlider.getX(), roomSizeSlider.getY() + labelYOffset, roomSizeSlider.getWidth(), labelHeight);
    dampLabel.setBounds(dampSlider.getX(), dampSlider.getY() + labelYOffset, dampSlider.getWidth(), labelHeight);
    wetLabel.setBounds(wetSlider.getX(), wetSlider.getY() + labelYOffset, wetSlider.getWidth(), labelHeight);
}

void ReverbComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::purple);
    g.setFont(juce::FontOptions(15.0f));
    auto bounds = getLocalBounds();
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 10.0f, 4.0f);

}