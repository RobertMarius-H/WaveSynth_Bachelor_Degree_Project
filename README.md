[README.md](https://github.com/user-attachments/files/34797098/README.md)
# WaveSynth

**WaveSynth** este un sintetizator software polifonic, realizat ca proiect de licență, dezvoltat în C++ cu framework-ul [JUCE](https://juce.com/) (v8.0.3). Poate fi rulat ca aplicație **Standalone** sau încărcat ca plugin **VST3 / AU** într-un DAW (Ableton, FL Studio, Reaper etc.).

Sintetizatorul combină două oscilatoare cu anvelope ADSR independente, un lanț de șase efecte audio, un filtru rezonant de tip ladder, un LFO pentru modulație și o interfață grafică cu vizualizarea în timp real a formei de undă.

---

## Funcționalități

### Sinteză
- **Polifonie de 16 voci** (`juce::Synthesiser` + `SynthVoice`)
- **2 oscilatoare** per voce, fiecare cu:
  - formă de undă selectabilă: **Sine**, **Saw**, **Square**
  - transpoziție în **octave** (−4 … +4) și **semitonuri** (−12 … +12)
  - **anvelopă ADSR** proprie (Attack, Decay, Sustain, Release)
  - **gain** individual (0 … 0.5)
- Intrare MIDI (controller extern sau tastatura virtuală din interfață)

### Lanț de efecte
Fiecare efect are un buton de activare (**Enable**) și un indicator de stare (cerc verde/roșu). Semnalul trece prin efecte în următoarea ordine:

```
Voci (Osc1 + Osc2) ─► Drive ─► Reverb ─► Flanger ─► Chorus ─► Phaser ─► Delay ─► Filter ─► Ieșire
                                                                                    ▲
                                                                     LFO (Filter / Volume)
```

| Efect | Implementare | Parametri |
|---|---|---|
| **Drive** | `juce::dsp::WaveShaper` | Tip (None / Soft / Hard / Arctangent), Drive, Mix |
| **Reverb** | `juce::dsp::Reverb` | Room Size, Damping, Wet Level |
| **Flanger** | Linie de întârziere modulată de un LFO sinusoidal (implementare proprie) | Rate, Depth, Feedback, Mix |
| **Chorus** | `juce::dsp::Chorus` | Rate, Depth, Centre Delay, Feedback, Mix |
| **Phaser** | Cascadă de filtre all-pass IIR modulate de LFO (implementare proprie) | Rate, Depth, Feedback, Mix |
| **Delay** | `juce::dsp::DelayLine` cu feedback (până la 2 s) | Time (ms), Feedback, Mix |

### Filtru
Filtru rezonant **ladder cu 4 etaje** (4 poli, în stilul filtrelor Moog), implementat de la zero în `FilterData`:
- Tipuri: **Low-Pass**, **Band-Pass**, **High-Pass**
- Cutoff (20 Hz – 22 kHz, scală logaritmică), Resonance, Mix

### LFO
- Forme de undă: **Sine**, **Square**, **Saw**, **Triangle**
- Destinație: **Filter** (modulează frecvența de cutoff) sau **Volume** (tremolo pe gain-ul oscilatoarelor)
- Parametri: Rate, Mix, Depth

### Interfață grafică
- Fereastră redimensionabilă (implicit 1500 × 760), temă întunecată
- Tastatură MIDI virtuală (`juce::MidiKeyboardComponent`)
- **Două osciloscoape** în timp real (60 FPS): semnalul brut generat de oscilatoare și semnalul final după efecte și filtru

---

## Arhitectură

Proiectul urmează un tipar consecvent: fiecare modul are o clasă **`*Data`** (procesarea DSP) și o clasă **`*Component`** (interfața grafică). Parametrii sunt centralizați în `juce::AudioProcessorValueTreeState` (APVTS), iar componentele grafice se leagă de ei prin *attachments*.

```
WaveSynth/
├── WaveSynth.jucer              # Proiectul Projucer (module, formate plugin, exportere)
├── Builds/VisualStudio2022/     # Soluția Visual Studio generată
├── JuceLibraryCode/             # Cod generat automat de Projucer
└── Source/
    ├── PluginProcessor.*        # Motorul audio: parametri, processBlock, lanțul de efecte
    ├── PluginEditor.*           # Fereastra principală și layout-ul UI
    ├── SynthSound.h             # Definirea sunetului (se aplică tuturor notelor/canalelor)
    ├── SynthVoice.*             # O voce: 2 oscilatoare + 2 ADSR + 2 gain
    ├── OscData / OscComponent   # Oscilator (juce::dsp::Oscillator) + selector formă de undă/pitch
    ├── AdsrData / AdsrComponent # Anvelopa ADSR
    ├── GainData / GainComponent # Volum per oscilator
    ├── DriveData / DriveComponent
    ├── ReverbData / ReverbComponent
    ├── ChorusData / ChorusComponent
    ├── FlangerData / FlangerComponent
    ├── PhaserData / PhaserComponent
    ├── DelayData / DelayComponent
    ├── FilterData / FilterComponent
    ├── LFO / LFOComponent
    ├── WaveformDisplay.*        # Osciloscop
    ├── SpectrumDisplay.*        # Analizor de spectru FFT (implementat, neinclus încă în UI)
    └── StatusCircle.*           # Indicator activ/inactiv pentru efecte
```

### Fluxul audio (`processBlock`)
1. Se citesc parametrii din APVTS și se actualizează fiecare voce (ADSR, formă de undă, transpoziție, gain — eventual modulat de LFO).
2. Mesajele MIDI (inclusiv cele de la tastatura virtuală) sunt trimise către `juce::Synthesiser`, care randează vocile active.
3. Semnalul brut este copiat în `oscBufferInitial` pentru vizualizare.
4. Se aplică efectele activate, în ordinea din diagrama de mai sus.
5. Rezultatul final este copiat în `oscBufferUI`, pe care editorul îl citește prin `Timer` la 60 Hz.

---

## Cerințe

- **Windows** 10/11
- **Visual Studio 2022** (workload „Desktop development with C++")
- **JUCE 8.0.3** + **Projucer**

## Compilare

1. Descarcă JUCE 8.0.3 și plasează-l astfel încât calea relativă din `.jucer` să fie validă (`../../../juce-8.0.3-windows/JUCE/modules` față de folderul `Builds/VisualStudio2022`), **sau** deschide `WaveSynth.jucer` în Projucer și setează calea către modulele JUCE din *Exporters → Visual Studio 2022 → Module Paths* (ori bifează *Use global path*).
2. Deschide `WaveSynth/WaveSynth.jucer` în Projucer și apasă **Save and Open in IDE**.
3. În Visual Studio, alege configurația (**Debug** / **Release**) și target-ul dorit:
   - `WaveSynth_StandalonePlugin` — aplicație de sine stătătoare
   - `WaveSynth_VST3` — plugin VST3
4. Compilează (`Ctrl+Shift+B`).

Fișierele rezultate se găsesc în `Builds/VisualStudio2022/x64/<Configurație>/`. Pentru a folosi pluginul într-un DAW, copiază `WaveSynth.vst3` în `C:\Program Files\Common Files\VST3\` și rescanează pluginurile.

> Formatul **AU** este activat în proiect, dar necesită un exporter Xcode (macOS), care nu este configurat momentan.

---

## Utilizare

1. Pornește aplicația Standalone (sau încarcă pluginul pe o pistă de instrument în DAW).
2. În Standalone, configurează ieșirea audio și (opțional) dispozitivul MIDI din **Options → Audio/MIDI Settings**.
3. Cântă folosind tastatura virtuală, tastatura calculatorului sau un controller MIDI.
4. Alege formele de undă, ajustează ADSR-ul și volumul celor două oscilatoare.
5. Activează efectele dorite cu butoanele **Enable** și reglează-le parametrii.

---

## Limitări cunoscute / direcții de dezvoltare

- `getStateInformation` / `setStateInformation` sunt goale — **setările nu se salvează** odată cu proiectul din DAW și nu există sistem de presete.
- Parametrul `MONO_MODE` este declarat, dar nu este folosit încă.
- `SpectrumDisplay` (analizor FFT) este implementat, dar nu este adăugat în interfață.
- Ordinea tipurilor de filtru din parametru (Low-Pass, Band-Pass, High-Pass) nu corespunde cu cea din `FilterData::processSample` (0 = LP, 1 = HP, 2 = BP), deci Band-Pass și High-Pass sunt inversate.
- În `processBlock`, `updateADSR` primește argumentele în ordinea (attack, **sustain, decay**, release), în timp ce funcția așteaptă (attack, **decay, sustain**, release) — Decay și Sustain sunt inversate.
- Valoarea LFO este calculată o singură dată per bloc audio (nu per eșantion), deci modulația este „în trepte" la rate mari.
- `SynthVoice::renderNextBlock` alocă buffere noi la fiecare apel, ceea ce nu este recomandat pe thread-ul audio.
- Variabila `lfoValue` nu este inițializată când LFO-ul este dezactivat.

---

## Autor

**Robert Hodoroaba** — proiect de licență.

Construit cu [JUCE](https://juce.com/).
