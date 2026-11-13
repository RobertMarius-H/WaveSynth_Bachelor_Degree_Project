/*
  ==============================================================================

    ChorusComponent.cpp
    Created: 4 Mar 2025 5:14:28pm
    Author:  rober

  ==============================================================================
*/

#include "ChorusComponent.h"

ChorusComponent::ChorusComponent(juce::AudioProcessorValueTreeState& apvts) {
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    rateAttachment = std::make_unique<SliderAttachment>(apvts, "RATE", rateSlider);
    depthAttachment = std::make_unique<SliderAttachment>(apvts, "DEPTH", depthSlider);
    delayCenterAttachment = std::make_unique<SliderAttachment>(apvts, "DELAYCENTER", delayCenterSlider);
    feedBackAttachment = std::make_unique<SliderAttachment>(apvts, "FEEDBACK", feedBackSlider);
    mixAttachment = std::make_unique<SliderAttachment>(apvts, "MIX", mixSlider);

    setupSliderWithLabel(rateSlider, rateLabel, "Rate(Hz)");
    setupSliderWithLabel(depthSlider, depthLabel, "Depth");
    setupSliderWithLabel(delayCenterSlider, delayCenterLabel, "Phase");
    setupSliderWithLabel(feedBackSlider, feedBackLabel, "Feedback");
    setupSliderWithLabel(mixSlider, mixLabel, "Mix");
}   

ChorusComponent::~ChorusComponent() {

}

void ChorusComponent::setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText) {
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

void ChorusComponent::resized() {

    const auto startY = 0;
    const auto sliderWidth = 70;
    const auto sliderHeight = 70;
    const auto labelYOffset = 70;
    const auto labelHeight = 20;

    //attackSlider.setBounds(0, startY + 5, sliderWidth, sliderHeight);
    //attackLabel.setBounds(attackSlider.getX(), attackSlider.getY() + labelYOffset , attackSlider.getWidth(), labelHeight);
    
    rateSlider.setBounds(0, 0, sliderWidth, sliderHeight);
    depthSlider.setBounds(rateSlider.getRight(), 0, sliderWidth, sliderHeight);
    delayCenterSlider.setBounds(depthSlider.getRight(), 0, sliderWidth, sliderHeight);
    feedBackSlider.setBounds(delayCenterSlider.getRight(), 0, sliderWidth, sliderHeight);
    mixSlider.setBounds(feedBackSlider.getRight(), 0, sliderWidth, sliderHeight);
    

    rateLabel.setBounds(rateSlider.getX(), rateSlider.getY() + labelYOffset, rateSlider.getWidth(), labelHeight);
    depthLabel.setBounds(depthSlider.getX(), depthSlider.getY() + labelYOffset, depthSlider.getWidth(), labelHeight);
    delayCenterLabel.setBounds(delayCenterSlider.getX(), delayCenterSlider.getY() + labelYOffset, delayCenterSlider.getWidth(), labelHeight);
    feedBackLabel.setBounds(feedBackSlider.getX(), feedBackSlider.getY() + labelYOffset, feedBackSlider.getWidth(), labelHeight);
    mixLabel.setBounds(mixSlider.getX(), mixSlider.getY() + labelYOffset, mixSlider.getWidth(), labelHeight);


}

void ChorusComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::purple);
    g.setFont(juce::FontOptions(15.0f));
    auto bounds = getLocalBounds();
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 10.0f, 4.0f);

}