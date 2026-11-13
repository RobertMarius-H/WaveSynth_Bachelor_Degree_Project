#pragma once

#include <JuceHeader.h>

class AdsrComponent : public juce::Component {

public:
    AdsrComponent(juce::AudioProcessorValueTreeState& apvts,const juce::String& prefix);
    ~AdsrComponent();

    void paint(juce::Graphics&) override;
    void resized() override;
    

private:
    void setupSilderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& labelText);

    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    //declarare action listeneruri
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdsrComponent)
};