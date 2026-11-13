/*
  ==============================================================================

    SynthVoice.cpp
    Created: 29 Oct 2024 10:50:52pm
    Author:  rober

  ==============================================================================
*/

#include "SynthVoice.h"
#include "SynthSound.h"
#include "PluginEditor.h"
#include "ReverbData.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}


void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchwheelPosition) {
    osc.setOscFrequency(midiNoteNumber);
    osc2.setOscFrequency(midiNoteNumber);


    adsr.noteOn();
    adsr2.noteOn();
}


void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    adsr.noteOff();
    adsr2.noteOff();
    if (!allowTailOff || (!adsr.isActive() && !adsr2.isActive()))
        clearCurrentNote();

}

void SynthVoice::controllerMoved(int controllerNumber, int newControllervalue) {

}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {

}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock,int outputChannels) {
      
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    synthBuffer.setSize(1, samplesPerBlock, false, true, true);
    synthBuffer2.setSize(1, samplesPerBlock, false, true, true);

    adsr.setSampleRate(sampleRate);
    adsr2.setSampleRate(sampleRate);
    
    osc.prepareToPlay(spec);
    osc2.prepareToPlay(spec);

    gain1.prepareToPlay(spec);
    gain2.prepareToPlay(spec);

    isPrepared = true;
}

void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release,
                            const float attack2, const float decay2, const float sustain2, const float release2) {
    adsr.updateADSR(attack, decay, sustain, release);
    adsr2.updateADSR(attack2, decay2, sustain2, release2);
}

void SynthVoice::updateGain(const float volume1, const float volume2) {
    gain1.updateVolume(volume1);
    gain2.updateVolume(volume2);
}

void SynthVoice::updateOffset(float newOffset1,float newOffset2) {
    osc.setFrequencyOffset(newOffset1);
    osc2.setFrequencyOffset(newOffset2);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) 
{
    jassert(isPrepared);

    if (!adsr.isActive() && !adsr2.isActive())
        return;

    juce::AudioBuffer<float> osc1Buffer(outputBuffer.getNumChannels(), numSamples);
    juce::AudioBuffer<float> osc2Buffer(outputBuffer.getNumChannels(), numSamples);

    osc1Buffer.clear();
    osc2Buffer.clear();


    for (int sample = 0; sample < numSamples; ++sample)
    {
        
        auto osc1Sample = osc.processSample(0.0f); // Generează un eșantion pentru Osc1
        auto osc2Sample = osc2.processSample(0.0f); // Generează un eșantion pentru Osc2

        auto env1Sample = adsr.getNextSample(); // Generează valoarea ADSR pentru Osc1
        auto env2Sample = adsr2.getNextSample(); // Generează valoarea ADSR pentru Osc2

        auto sampleValue1 = osc1Sample * env1Sample; // Aplică ADSR pentru Osc1
        auto sampleValue2 = osc2Sample * env2Sample; // Aplică ADSR pentru Osc2
       
        //auto mixedSample = (sampleValue1 + sampleValue2) * 0.5f; // Mixăm cele două oscilatoare

        
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            osc1Buffer.addSample(channel, sample, sampleValue1);
            osc2Buffer.addSample(channel, sample, sampleValue2);
        }
    }

    juce::dsp::AudioBlock<float> osc1Block(osc1Buffer);
    juce::dsp::AudioBlock<float> osc2Block(osc2Buffer);

    gain1.processNextBlock(osc1Block);
    gain2.processNextBlock(osc2Block);

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample, osc1Buffer, channel, 0, numSamples);
        outputBuffer.addFrom(channel, startSample, osc2Buffer, channel, 0, numSamples);
    }

    if (!adsr.isActive() && !adsr2.isActive())
    {
        clearCurrentNote();
    }
}
