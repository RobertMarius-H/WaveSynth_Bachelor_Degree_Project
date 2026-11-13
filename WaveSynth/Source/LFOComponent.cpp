/*
  ==============================================================================

    LFOComponent.cpp
    Created: 4 Apr 2025 4:28:00pm
    Author:  rober

  ==============================================================================
*/

#include "LFOComponent.h"

LFOComponent::LFOComponent(juce::AudioProcessorValueTreeState& apvts)
{

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    waveformSelector.addItemList({ "Sine", "Square", "Saw", "Triangle" }, 1);
    waveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "LFO_WAVEFORM", waveformSelector);
    addAndMakeVisible(waveformSelector);
    addAndMakeVisible(waveformLabel);
    addAndMakeVisible(targetSelector);
    addAndMakeVisible(targetLabel);

    targetSelector.addItemList({ "Filter", "Volume" }, 1);
    targetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "LFO_TARGET", targetSelector);
    rateAttachment = std::make_unique<SliderAttachment>(apvts, "LFO_RATE", rateSlider);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "LFO_MIX", mixSlider);
    depthAttachment= std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "LFO_DEPTH", depthSlider);
    setupSliderWithLabel(mixSlider, mixLabel, "Mix");
    setupSliderWithLabel(rateSlider, rateLabel, "Rate");
    setupSliderWithLabel(depthSlider, depthLabel, "Depth");
}

void LFOComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::purple);
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 10.0f, 4.0f);
}

void LFOComponent::resized()
{
    const auto sliderWidth = 70;
    const auto sliderHeight = 70;
    const auto labelYOffset = 70;
    const auto labelHeight = 20;
    const auto comboBoxHeight = 30;

    auto area = getLocalBounds().reduced(10);
    auto rowHeight = 30;

    mixSlider.setBounds(0, 0, sliderWidth, sliderHeight);
    mixLabel.setBounds(mixSlider.getX(), mixSlider.getBottom(), mixSlider.getWidth(), labelHeight);

    rateSlider.setBounds(mixSlider.getRight(),mixSlider.getY(),sliderWidth,sliderHeight);
    rateLabel.setBounds(rateSlider.getX(), rateSlider.getBottom(), mixSlider.getWidth(), labelHeight);

    depthSlider.setBounds(rateSlider.getRight(), rateSlider.getY(), sliderWidth, sliderHeight);
    depthLabel.setBounds(depthSlider.getX(),depthSlider.getBottom(), mixSlider.getWidth(), labelHeight);

    waveformSelector.setBounds(depthSlider.getRight() + 10, 35, 100, comboBoxHeight);
    waveformLabel.setBounds(waveformSelector.getX(), waveformSelector.getBottom(), mixSlider.getWidth(), labelHeight);

    targetSelector.setBounds(waveformSelector.getRight() + 10, 35, 100, comboBoxHeight);
    targetLabel.setBounds(targetSelector.getX(), targetSelector.getBottom() + 20, mixSlider.getWidth(), labelHeight);
}

void LFOComponent::setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText)
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