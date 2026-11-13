/*
  ==============================================================================

    OscData.cpp
    Created: 7 Dec 2024 12:36:35pm
    Author:  rober

  ==============================================================================
*/

#include "OscData.h"

void OscData::setWaveType(const int choice) {

    switch (choice) {
    case 0:
        initialise([](float x) {return std::sin(x); }); //sin
        break;

    case 1:
        initialise([](float x) {return x / juce::MathConstants<float>::pi; }); //saw
        break;

    case 2:
        initialise([](float x) {return x < 0.0f ? -1.0f : 1.0f; }); //square
        break;

    default:
        jassertfalse; //inchide pluginul in cazul unei erori
        break;
    }
}


//pregateste oscilatorul pentru a reda sunete
void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec) {
    prepare(spec);
}

//calculeaza urmatoarea informatie audio si o proceseaza pentru redare
void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block) {
    process(juce::dsp::ProcessContextReplacing<float>(block));
}

//seteaza frecventa notelor cu midiNoteNumber
void OscData::setOscFrequency(const int midiNoteNumber)
{
    baseFrequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    float freq = baseFrequency * std::pow(2.0f, offset / 12.0f);  
    setFrequency(freq);
}

void OscData::setFrequencyOffset(float newOffset)
{
    offset = newOffset;
}