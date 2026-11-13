#pragma once
#include <JuceHeader.h>
#include "FilterData.h"

class FilterComponent : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& apvt);
    ~FilterComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void setupSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText);

    juce::Slider cutoffSlider;
    juce::Slider resonanceSlider;
    juce::Slider mixSlider;

    juce::Label cutoffLabel;
    juce::Label resonanceLabel;
    juce::Label mixLabel;

    juce::ComboBox filterTypeBox;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    std::unique_ptr<SliderAttachment> cutoffAttachment;
    std::unique_ptr<SliderAttachment> resonanceAttachment;
    std::unique_ptr<ComboBoxAttachment> filterTypeAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterComponent)
};
