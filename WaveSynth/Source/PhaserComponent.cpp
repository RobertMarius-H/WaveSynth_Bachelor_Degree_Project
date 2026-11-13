/*
  ==============================================================================

    PhaserComponent.cpp
    Created: 12 Mar 2025 12:57:11pm
    Author:  rober

  ==============================================================================
*/

#include "PhaserComponent.h"

PhaserComponent::PhaserComponent(juce::AudioProcessorValueTreeState& apvts) {
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    rateAttachment = std::make_unique<SliderAttachment>(apvts, "PHASER_RATE", rateSlider);
    depthAttachment = std::make_unique<SliderAttachment>(apvts, "PHASER_DEPTH", depthSlider);
    feedBackAttachment = std::make_unique<SliderAttachment>(apvts, "PHASER_FEEDBACK", feedBackSlider);
    mixAttachment = std::make_unique<SliderAttachment>(apvts, "PHASER_MIX", mixSlider);

    setupSliderWithLabel(rateSlider, rateLabel, "Rate");
    setupSliderWithLabel(depthSlider, depthLabel, "Depth");
    setupSliderWithLabel(feedBackSlider, feedBackLabel, "Feedback");
    setupSliderWithLabel(mixSlider, mixLabel, "Mix");
}

PhaserComponent::~PhaserComponent() {

}

void PhaserComponent::setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText) {
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

void PhaserComponent::resized() {

    const auto startY = 0;
    const auto sliderWidth = 70;
    const auto sliderHeight = 70;
    const auto labelYOffset = 70;
    const auto labelHeight = 20;

    //attackSlider.setBounds(0, startY + 5, sliderWidth, sliderHeight);
    //attackLabel.setBounds(attackSlider.getX(), attackSlider.getY() + labelYOffset , attackSlider.getWidth(), labelHeight);
    rateSlider.setBounds(0, 0, sliderWidth, sliderHeight);
    depthSlider.setBounds(rateSlider.getRight(), 0, sliderWidth, sliderHeight);
    feedBackSlider.setBounds(depthSlider.getRight(), 0, sliderWidth, sliderHeight);
    mixSlider.setBounds(feedBackSlider.getRight(), 0, sliderWidth, sliderHeight);

    rateLabel.setBounds(rateSlider.getX(), rateSlider.getY() + labelYOffset, rateSlider.getWidth(), labelHeight);
    depthLabel.setBounds(depthSlider.getX(), depthSlider.getY() + labelYOffset, depthSlider.getWidth(), labelHeight);
    feedBackLabel.setBounds(feedBackSlider.getX(), feedBackSlider.getY() + labelYOffset, feedBackSlider.getWidth(), labelHeight);
    mixLabel.setBounds(mixSlider.getX(), mixSlider.getY() + labelYOffset, mixSlider.getWidth(), labelHeight);

}

void PhaserComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::purple);
    g.setFont(juce::FontOptions(15.0f));
    auto bounds = getLocalBounds();
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 10.0f, 4.0f);

}