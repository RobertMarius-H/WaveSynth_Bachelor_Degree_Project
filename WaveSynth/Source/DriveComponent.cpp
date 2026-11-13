/*
  ==============================================================================

    DriveComponent.cpp
    Created: 26 Feb 2025 10:13:56am
    Author:  rober

  ==============================================================================
*/

#include "DriveComponent.h"


DriveComponent::DriveComponent(juce::AudioProcessorValueTreeState& apvts, juce::String distType) {
    juce::StringArray distortionType{ "None", "Soft", "Hard", "Atan"};
    distSelector.addItemList(distortionType, 1);
    addAndMakeVisible(distSelector);

    distSelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, distType, distSelector);
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    driveAttachment = std::make_unique<SliderAttachment>(apvts, "DRIVE", driveSlider);
    mixAttachment = std::make_unique<SliderAttachment>(apvts, "DRIVE_MIX", mixSlider);

    setupSliderWithLabel(driveSlider, driveLabel, "Drive");
    //setupSliderWithLabel(mixSlider, mixLabel, "Mix");
}

void DriveComponent::setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText) {
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

void DriveComponent::resized() {

    const auto startY = 0;
    const auto sliderWidth = 70;
    const auto sliderHeight = 70;
    const auto labelYOffset = 70;
    const auto labelHeight = 20;


    driveSlider.setBounds(0, 0, sliderWidth, sliderHeight);
    driveLabel.setBounds(driveSlider.getX(), driveSlider.getY() + labelYOffset, driveSlider.getWidth(), labelHeight);
    distSelector.setBounds(driveSlider.getRight()+10,driveSlider.getY() + 30, driveLabel.getWidth(),labelHeight);

    //mixSlider.setBounds(driveSlider.getRight(), 0, sliderWidth, sliderHeight);
    //mixLabel.setBounds(mixSlider.getX(), mixSlider.getY() + labelYOffset, driveSlider.getWidth(), labelHeight);
}


void DriveComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::purple);
    g.setFont(juce::FontOptions(15.0f));
    auto bounds = getLocalBounds();
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 10.0f, 4.0f);
}
