/*
  ==============================================================================

    DelayComponent.cpp
    Created: 12 Mar 2025 12:57:25pm
    Author:  rober

  ==============================================================================
*/

#include "DelayComponent.h"


DelayComponent::DelayComponent(juce::AudioProcessorValueTreeState& apvts) {
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    delayAttachment = std::make_unique<SliderAttachment>(apvts, "DELAY_DEPTH", delaySlider);
    feedbackAttachment = std::make_unique<SliderAttachment>(apvts, "DELAY_FEEDBACK", feedbackSlider);
    mixAttachment = std::make_unique<SliderAttachment>(apvts, "DELAY_MIX", mixSlider);

    setupSliderWithLabel(delaySlider, delayLabel, "Delay");
    setupSliderWithLabel(feedbackSlider, feedbackLabel, "Feedback");
    setupSliderWithLabel(mixSlider, mixLabel, "Mix");
}

DelayComponent::~DelayComponent() {

}

void DelayComponent::setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText) {
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

void DelayComponent::resized() {

    const auto startY = 0;
    const auto sliderWidth = 70;
    const auto sliderHeight = 70;
    const auto labelYOffset = 70;
    const auto labelHeight = 20;

    //attackSlider.setBounds(0, startY + 5, sliderWidth, sliderHeight);
    //attackLabel.setBounds(attackSlider.getX(), attackSlider.getY() + labelYOffset , attackSlider.getWidth(), labelHeight);
    delaySlider.setBounds(0, 0, sliderWidth, sliderHeight);
    feedbackSlider.setBounds(delaySlider.getRight(), 0, sliderWidth, sliderHeight);
    mixSlider.setBounds(feedbackSlider.getRight(), 0, sliderWidth, sliderHeight);

    delayLabel .setBounds(delaySlider.getX(), delaySlider.getY() + labelYOffset, delaySlider.getWidth(), labelHeight);
    feedbackLabel.setBounds(feedbackSlider.getX(), feedbackSlider.getY() + labelYOffset, feedbackSlider.getWidth(), labelHeight);
    mixLabel.setBounds(mixSlider.getX(), mixSlider.getY() + labelYOffset, mixSlider.getWidth(), labelHeight);

}

void DelayComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::purple);
    g.setFont(juce::FontOptions(15.0f));
    auto bounds = getLocalBounds();
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 10.0f, 4.0f);

}