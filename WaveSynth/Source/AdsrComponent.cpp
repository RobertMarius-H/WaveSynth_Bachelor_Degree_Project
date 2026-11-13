/*
  ==============================================================================

    AdsrComponent.cpp
    Created: 19 Nov 2024 6:01:58pm
    Author:  rober

  ==============================================================================
*/

#include "AdsrComponent.h"
#include <JuceHeader.h>

AdsrComponent::AdsrComponent(juce::AudioProcessorValueTreeState& apvts,const juce::String& prefix) {
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    attackAttachment = std::make_unique<SliderAttachment>(apvts, "ATTACK" + prefix, attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(apvts, "DECAY" + prefix, decaySlider);
    sustainAttachment = std::make_unique<SliderAttachment>(apvts, "SUSTAIN" + prefix, sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(apvts, "RELEASE" + prefix, releaseSlider);

    setupSilderWithLabel(attackSlider, attackLabel, "Attack");
    setupSilderWithLabel(decaySlider, decayLabel, "Decay");
    setupSilderWithLabel(sustainSlider, sustainLabel, "Sustain");
    setupSilderWithLabel(releaseSlider, releaseLabel, "Release");
}

AdsrComponent::~AdsrComponent() {

}

void AdsrComponent::setupSilderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText) {
    
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


void AdsrComponent::resized() {
    const auto startY = 0;
    const auto sliderWidth = 70;
    const auto sliderHeight = 70;
    const auto labelYOffset = 70;
    const auto labelHeight = 20;

    attackSlider.setBounds(0, startY + 5, sliderWidth, sliderHeight);
    attackLabel.setBounds(attackSlider.getX(), attackSlider.getY() + labelYOffset , attackSlider.getWidth(), labelHeight);

    decaySlider.setBounds(attackSlider.getRight(), startY + 5, sliderWidth, sliderHeight);
    decayLabel.setBounds(decaySlider.getX(), decaySlider.getY() + labelYOffset, decaySlider.getWidth(), labelHeight);

    sustainSlider.setBounds(decaySlider.getRight(), startY + 5, sliderWidth, sliderHeight);
    sustainLabel.setBounds(sustainSlider.getX(), sustainSlider.getY() + labelYOffset, sustainSlider.getWidth(), labelHeight);

    releaseSlider.setBounds(sustainSlider.getRight(), startY + 5, sliderWidth, sliderHeight);
    releaseLabel.setBounds(releaseSlider.getX(), releaseSlider.getY() + labelYOffset, releaseSlider.getWidth(), labelHeight);
}


void AdsrComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::purple);
    g.setFont(juce::FontOptions(15.0f));
    auto bounds = getLocalBounds();
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 10.0f, 4.0f);
}