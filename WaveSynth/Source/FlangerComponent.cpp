/*
  ==============================================================================

    FlangerComponent.cpp
    Created: 5 Mar 2025 6:13:40pm
    Author:  rober

  ==============================================================================
*/

#include "FlangerComponent.h"


FlangerComponent::FlangerComponent(juce::AudioProcessorValueTreeState& apvts) {
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    rateAttachment = std::make_unique<SliderAttachment>(apvts, "FLANGER_RATE", rateSlider);
    depthAttachment = std::make_unique<SliderAttachment>(apvts, "FLANGER_DEPTH", depthSlider);
    feedbackAttachment = std::make_unique<SliderAttachment>(apvts, "FLANGER_FEEDBACK", feedbackSlider);
    wetAttachment = std::make_unique<SliderAttachment>(apvts, "FLANGER_MIX", wetSlider);

    setupSliderWithLabel(rateSlider, rateLabel, "Rate");
    setupSliderWithLabel(depthSlider, depthLabel, "Depth");
    setupSliderWithLabel(feedbackSlider, feedbackLabel, "Feedback");
    setupSliderWithLabel(wetSlider, wetLabel, "Mix");
}

FlangerComponent::~FlangerComponent() {

}

void FlangerComponent::setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText) {
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


void FlangerComponent::resized() {

    const auto startY = 0;
    const auto sliderWidth = 70;
    const auto sliderHeight = 70;
    const auto labelYOffset = 70;
    const auto labelHeight = 20;

    //attackSlider.setBounds(0, startY + 5, sliderWidth, sliderHeight);
    //attackLabel.setBounds(attackSlider.getX(), attackSlider.getY() + labelYOffset , attackSlider.getWidth(), labelHeight);

    rateSlider.setBounds(0, 0, sliderWidth, sliderHeight);
    depthSlider.setBounds(rateSlider.getRight(), 0, sliderWidth, sliderHeight);
    feedbackSlider.setBounds(depthSlider.getRight(), 0, sliderWidth, sliderHeight);
    wetSlider.setBounds(feedbackSlider.getRight(), 0, sliderWidth, sliderHeight);

    rateLabel.setBounds(rateSlider.getX(), rateSlider.getY() + labelYOffset, rateSlider.getWidth(), labelHeight);
    depthLabel.setBounds(depthSlider.getX(), depthSlider.getY() + labelYOffset, depthSlider.getWidth(), labelHeight);
    feedbackLabel.setBounds(feedbackSlider.getX(), feedbackSlider.getY() + labelYOffset, feedbackSlider.getWidth(), labelHeight);
    wetLabel.setBounds(wetSlider.getX(), wetSlider.getY() + labelYOffset, wetSlider.getWidth(), labelHeight);
}

void FlangerComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::purple);
    g.setFont(juce::FontOptions(15.0f));
    auto bounds = getLocalBounds();
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 10.0f, 4.0f);

}