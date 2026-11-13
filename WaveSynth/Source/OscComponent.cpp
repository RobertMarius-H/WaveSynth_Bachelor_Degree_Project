/*
  ==============================================================================

    OscComponent.cpp
    Created: 21 Nov 2024 10:30:02pm
    Author:  rober

  ==============================================================================
*/

/*
using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    delayAttachment = std::make_unique<SliderAttachment>(apvts, "DELAY_DEPTH", delaySlider);
    feedbackAttachment = std::make_unique<SliderAttachment>(apvts, "DELAY_FEEDBACK", feedbackSlider);
    mixAttachment = std::make_unique<SliderAttachment>(apvts, "DELAY_MIX", mixSlider);
*/

#include "OscComponent.h"

OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorID, juce::String oscID) {

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    juce::StringArray choices{ "Sine", "Saw", "Square" };
    oscSelector.addItemList(choices, 1);
    addAndMakeVisible(oscSelector);

    //conecteaza alegerile din comboBox si sincronizeaza cu audioProcessor
    oscSelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorID,oscSelector);
    tonesAttachment = std::make_unique<SliderAttachment>(apvts, "OSCTONES" + oscID, toneSlider);
    semitonesAttachment = std::make_unique<SliderAttachment>(apvts, "OSCSEMITONES" + oscID, semitoneSlider);

    setupSliderWithLabel(toneSlider, toneLabel, "Octaves");
    setupSliderWithLabel(semitoneSlider, semitoneLabel, "Semitones");


}

void OscComponent::setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText) {

    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::blueviolet);
    slider.setColour(juce::Slider::thumbColourId, juce::Colours::violet);

    addAndMakeVisible(label);
    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colours::white);
}

OscComponent::~OscComponent() {

}

void OscComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::purple);
    g.setFont(juce::FontOptions(15.0f));
    auto bounds = getLocalBounds();
    g.drawRoundedRectangle(bounds.toFloat(), 10.0f, 4.0f);
}

void OscComponent::resized() {

    const auto padding = 10;
    const auto startY = 0;
    const auto sliderWidth = 70;
    const auto sliderHeight = 70;
    const auto labelYOffset = 70;
    const auto labelHeight = 20;
    
    oscSelector.setBounds(10, 30, 90, 30);

    toneSlider.setBounds(oscSelector.getRight() + padding, 0, sliderWidth, sliderHeight);
    toneLabel.setBounds(toneSlider.getX(), toneSlider.getBottom(), sliderWidth, labelHeight);

    semitoneSlider.setBounds(toneSlider.getRight() + padding, 0 , sliderWidth, sliderHeight);
    semitoneLabel.setBounds(semitoneSlider.getX(), semitoneSlider.getBottom(), sliderWidth, labelHeight);
}


