/*
  ==============================================================================

    FilterComponent.cpp
    Created: 19 Mar 2025 6:32:39pm
    Author:  rober

  ==============================================================================
*/

#include "FilterComponent.h"

FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts)
{
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    cutoffAttachment = std::make_unique<SliderAttachment>(apvts, "FILTER_CUTOFF", cutoffSlider);
    resonanceAttachment = std::make_unique<SliderAttachment>(apvts, "FILTER_RESONANCE", resonanceSlider);
    mixAttachment = std::make_unique<SliderAttachment>(apvts, "FILTER_MIX", mixSlider);

    setupSliderWithLabel(cutoffSlider, cutoffLabel, "Cutoff");
    setupSliderWithLabel(resonanceSlider, resonanceLabel, "Resonance");
    setupSliderWithLabel(mixSlider, mixLabel,"Mix");

    juce::StringArray choices{ "Low-Pass", "High-Pass", "Band-Pass" };
    filterTypeBox.addItemList(choices, 1);
    filterTypeAttachment = std::make_unique<ComboBoxAttachment>(apvts, "FILTER_TYPE", filterTypeBox);

    addAndMakeVisible(filterTypeBox);

}

FilterComponent::~FilterComponent() {}

void FilterComponent::setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText)
{
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

void FilterComponent::resized()
{
    const auto sliderWidth = 70;
    const auto sliderHeight = 70;
    const auto labelYOffset = 70;
    const auto labelHeight = 20;
    const auto comboBoxHeight = 30;

    cutoffSlider.setBounds(0, 0, sliderWidth, sliderHeight);
    resonanceSlider.setBounds(cutoffSlider.getRight(), 0, sliderWidth, sliderHeight);
    mixSlider.setBounds(resonanceSlider.getRight(), 0, sliderWidth, sliderHeight);

    cutoffLabel.setBounds(cutoffSlider.getX(), cutoffSlider.getY() + labelYOffset, cutoffSlider.getWidth(), labelHeight);
    resonanceLabel.setBounds(resonanceSlider.getX(), resonanceSlider.getY() + labelYOffset, resonanceSlider.getWidth(), labelHeight);
    mixLabel.setBounds(mixSlider.getX(), mixSlider.getY() + labelYOffset, mixSlider.getWidth(), labelHeight);

    filterTypeBox.setBounds(mixSlider.getRight() + 10, 35, 100, comboBoxHeight);

}

void FilterComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::purple);
    g.setFont(juce::FontOptions(15.0f));
    auto bounds = getLocalBounds();
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 10.0f, 4.0f);

}


