/*
  ==============================================================================

    SynthVoice.h
    Created: 29 Oct 2024 10:50:52pm
    Author:  rober

  ==============================================================================
*/

#pragma once
#include "SynthSound.h"
#include "JuceHeader.h"
#include "AdsrData.h"
#include "OscData.h"
#include "ReverbData.h"
#include "GainData.h"


class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchwheelPosition)override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllervalue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    void pitchWheelMoved(int newPitchWheelValue) override;

    void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release,
                                const float attack2, const float decay2, const float sustain2, const float release2);

    void SynthVoice::updateGain(const float volume1, const float volume2);
    void updateOffset(float newOffset1, float newOffset2);

    OscData& getOscillator() { return osc; }
    OscData& getOscillator2() { return osc2; }



private:
    juce::AudioBuffer<float> synthBuffer;
    juce::AudioBuffer<float> synthBuffer2;


    AdsrData adsr;
    AdsrData adsr2;

    OscData osc;
    OscData osc2;

    GainData gain1, gain2;
    float totalOffset1 =0.0f;
    float totalOffset2 = 0.0f;

    bool isPrepared{ false };
};