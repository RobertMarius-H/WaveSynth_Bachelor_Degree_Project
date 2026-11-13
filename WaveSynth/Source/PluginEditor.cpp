/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
WaveSynthAudioProcessorEditor::WaveSynthAudioProcessorEditor(WaveSynthAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    osc(audioProcessor.apvts, "OSC1WAVETYPE","1"),
    osc2(audioProcessor.apvts, "OSC2WAVETYPE","2"),
    adsr(audioProcessor.apvts, "1"),
    adsr2(audioProcessor.apvts, "2"),
    keyboardComponent(audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
    reverb(audioProcessor.apvts),
    drive(audioProcessor.apvts, "DISTTYPE"),
    gain1(audioProcessor.apvts, "1"),
    gain2(audioProcessor.apvts, "2"),
    chorus(audioProcessor.apvts),
    flanger(audioProcessor.apvts),
    phaser(audioProcessor.apvts),
    delay(audioProcessor.apvts),
    filter(audioProcessor.apvts),
    lfo(audioProcessor.apvts)
{
    setSize (1500, 760);
    setResizable(true,true);
    addAndMakeVisible(adsr);
    addAndMakeVisible(adsr2);

    addAndMakeVisible(osc);
    addAndMakeVisible(osc2);
    
    addAndMakeVisible(reverb);
    addAndMakeVisible(drive);
    addAndMakeVisible(chorus);
    addAndMakeVisible(flanger);
    addAndMakeVisible(phaser);
    addAndMakeVisible(delay);

    addAndMakeVisible(gain1);
    addAndMakeVisible(gain2);

    addAndMakeVisible(delayButton);
    addAndMakeVisible(reverbButton);
    addAndMakeVisible(phaserButton);
    addAndMakeVisible(flangerButton);
    addAndMakeVisible(chorusButton);
    addAndMakeVisible(driveButton);
    addAndMakeVisible(filterButton);
    addAndMakeVisible(lfoButton);
    
    addAndMakeVisible(filter);
    addAndMakeVisible(lfo);
//////////////////////////////////////////////

    addAndMakeVisible(reverbCircle);
    addAndMakeVisible(phaserCircle);
    addAndMakeVisible(flangerCircle);
    addAndMakeVisible(delayCircle);
    addAndMakeVisible(driveCircle);
    addAndMakeVisible(chorusCircle);
    addAndMakeVisible(lfoCircle);
    addAndMakeVisible(filterCircle);

    reverbButton.onClick = [this]() {
        reverbCircle.setActive(reverbButton.getToggleState());
        };

    reverbButton.onClick = [this]() {
        reverbCircle.setActive(reverbButton.getToggleState());
        };

    phaserButton.onClick = [this]() {
        phaserCircle.setActive(phaserButton.getToggleState());
        };

    flangerButton.onClick = [this]() {
        flangerCircle.setActive(flangerButton.getToggleState());
        };

    delayButton.onClick = [this]() {
        delayCircle.setActive(delayButton.getToggleState());
        };

    driveButton.onClick = [this]() {
        driveCircle.setActive(driveButton.getToggleState());
        };

    chorusButton.onClick = [this]() {
        chorusCircle.setActive(chorusButton.getToggleState());
        };

    lfoButton.onClick = [this]() {
        lfoCircle.setActive(lfoButton.getToggleState());
        };

    filterButton.onClick = [this]() {
        filterCircle.setActive(filterButton.getToggleState());
        };
/////////////////////////////////////////////

    delayButton.setButtonText("Enable Delay");
    delayEnAtt = std::make_unique<juce::ButtonParameterAttachment>(*audioProcessor.apvts.getParameter("DELAY_ENABLED"), delayButton);
    
    reverbButton.setButtonText("Enable Reverb");
    reverbEnAtt = std::make_unique<juce::ButtonParameterAttachment>(*audioProcessor.apvts.getParameter("REVERB_ENABLED"), reverbButton);

    phaserButton.setButtonText("Enable Phaser");
    phaserEnAtt = std::make_unique<juce::ButtonParameterAttachment>(*audioProcessor.apvts.getParameter("PHASER_ENABLED"), phaserButton);

    flangerButton.setButtonText("Enable Flanger");
    flangerEnAtt = std::make_unique<juce::ButtonParameterAttachment>(*audioProcessor.apvts.getParameter("FLANGER_ENABLED"), flangerButton);

    chorusButton.setButtonText("Enable Chorus");
    chorusEnAtt = std::make_unique<juce::ButtonParameterAttachment>(*audioProcessor.apvts.getParameter("CHORUS_ENABLED"), chorusButton);

    driveButton.setButtonText("Enable Drive");
    driveEnAtt = std::make_unique<juce::ButtonParameterAttachment>(*audioProcessor.apvts.getParameter("DRIVE_ENABLED"), driveButton);
    
    filterButton.setButtonText("Enable Filter");
    filterEnAtt = std::make_unique<juce::ButtonParameterAttachment>(*audioProcessor.apvts.getParameter("FILTER_ENABLED"), filterButton);

    lfoButton.setButtonText("Enable LFO");
    lfoEnAtt = std::make_unique<juce::ButtonParameterAttachment>(*audioProcessor.apvts.getParameter("LFO_ENABLED"), lfoButton);

    addAndMakeVisible(keyboardComponent);

    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(waveformInitial);
    startTimerHz(60);
}

WaveSynthAudioProcessorEditor::~WaveSynthAudioProcessorEditor()
{
}

//==============================================================================
void WaveSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void WaveSynthAudioProcessorEditor::resized()
{
    const auto paddingX = 10;
    const auto paddingY = 60;
    const auto paddingY2 = 235;

    osc.setBounds(10, 40, 300, 100);
    osc2.setBounds(osc.getRight() + 90, 40, 300, 100);

    adsr.setBounds(paddingX, osc.getBottom() + 30,280 ,100);
    adsr2.setBounds(osc2.getX(), osc2.getBottom() + 30, 280, 100);

    gain1.setBounds(adsr.getRight() + 10, adsr.getY(), 90, 100);
    gain2.setBounds(adsr2.getRight() + 10, adsr2.getY(), 90, 100);

    //UI Efecte
    reverb.setBounds(paddingX,adsr.getBottom() + 30, 280, 100);
    drive.setBounds(paddingX, reverb.getBottom() + 30,200, 100);
    chorus.setBounds(paddingX, drive.getBottom() + 30, 350, 100);
    flanger.setBounds(adsr2.getX(), adsr2.getBottom() + 30, 280, 100);
    phaser.setBounds(adsr2.getX(), flanger.getBottom() + 30, 280, 100);
    delay.setBounds(adsr2.getX(), phaser.getBottom() + 30, 210, 100);
    filter.setBounds(delay.getRight() + 20, delay.getY(), 350, 100);
    lfo.setBounds(filter.getRight() + 10, filter.getY(), 450, 100);
    //

    //Butoane
    filterButton.setBounds(filter.getX(), filter.getY() - 35, 100, 40);
    reverbButton.setBounds(reverb.getX(), reverb.getY() - 35 , 100, 40);
    phaserButton.setBounds(phaser.getX(), phaser.getY() - 35, 100, 40);
    flangerButton.setBounds(flanger.getX(), flanger.getY() - 35, 100, 40);
    chorusButton.setBounds(chorus.getX(), chorus.getY() - 35, 100, 40);
    driveButton.setBounds(drive.getX(), drive.getY() - 35, 100, 40);
    delayButton.setBounds(delay.getX(), delay.getY() - 35, 100, 40);
    lfoButton.setBounds(lfo.getX(), lfo.getY() - 35, 100, 40);

    //Cercuri status
    reverbCircle.setBounds(reverbButton.getRight() + 10, reverbButton.getY() + 10, 20, 20);
    delayCircle.setBounds(delayButton.getRight() + 10, delayButton.getY() + 10, 20, 20);
    phaserCircle.setBounds(phaserButton.getRight() + 10, phaserButton.getY() + 10, 20, 20);
    flangerCircle.setBounds(flangerButton.getRight() + 10, flangerButton.getY() + 10, 20, 20);
    chorusCircle.setBounds(chorusButton.getRight() + 10, chorusButton.getY() + 10, 20, 20);
    driveCircle.setBounds(driveButton.getRight() + 10, driveButton.getY() + 10, 20, 20);
    filterCircle.setBounds(filterButton.getRight() + 10, filterButton.getY() + 10, 20, 20);
    lfoCircle.setBounds(lfoButton.getRight() + 10, lfoButton.getY() + 10, 20, 20);

    keyboardComponent.setBounds(paddingX + 150, filter.getBottom() + 10, 1200, 80);

    //display
    waveformDisplay.setBounds(osc2.getRight() + 120, osc2.getY(), 600, 230);
    waveformInitial.setBounds(osc2.getRight() + 120, waveformDisplay.getBottom() + 20, 600, 230);
}

void WaveSynthAudioProcessorEditor::timerCallback()
{
    waveformDisplay.setBuffer(audioProcessor.oscBufferUI);
    waveformInitial.setBuffer(audioProcessor.oscBufferInitial);

}





