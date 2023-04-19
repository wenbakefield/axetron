// Title: AxeTron
// Description: Onboard multieffects for electric guitar
// Hardware: Daisy Seed
// Author: Ben Wakefield

#include "DaisyDuino.h"

DaisyHardware hw;

Limiter limiter;
Flanger flanger;
Phaser phaser;
Tremolo tremolo;
Autowah autowah;
Compressor compressor;
ReverbSc reverb;

float knob1;
float knob2;
float knob3;
float knob4;
float knob5;
float knob6;

Switch button1;
Switch button2;
Switch button3;

AnalogBassDrum bassdrum;
AnalogSnareDrum snaredrum;
HiHat<> hihat;

void AudioCallback(float **in, float **out, size_t size) {
  for (size_t i = 0; i < size; i++) {
    float input_sample = in[0][i] * 8;
    float output_sample = input_sample;
    if (knob1 > 0.01) {
      output_sample = fuzz(output_sample, knob1);
    }
    if (knob2 > 0.01) {
      flanger.SetLfoDepth(knob2);
      output_sample = flanger.Process(output_sample);
    }
    if (knob3 > 0.01) {
      phaser.SetLfoDepth(knob3);
      output_sample = phaser.Process(output_sample);
    }
    if (knob4 > 0.01) {
      compressor.SetThreshold(knob4 * -80.0);
      output_sample = compressor.Process(output_sample);
    }
    if (knob5 > 0.01) {
      tremolo.SetDepth(knob5);
      output_sample = tremolo.Process(output_sample);
    }
    if (knob6 > 0.01) {
      autowah.SetDryWet(knob6 * 100);
      output_sample = autowah.Process(output_sample);
    }

    // if (button1.RisingEdge()) {
    //   output_sample = output_sample + hihat.Process(true);
    // }
    // if (button2.RisingEdge()) {
    //   output_sample = output_sample + bassdrum.Process(true);
    // }
    // if (button3.RisingEdge()) {
    //   output_sample = output_sample + snaredrum.Process(true);
    // }

    out[0][i] = out[1][i] = output_sample;
  }

  //limiter.ProcessBlock(out[0], size, 0.0);
  //limiter.ProcessBlock(out[1], size, 0.0);
}

void setup() {
  float sample_rate;
  hw = DAISY.init(DAISY_SEED, AUDIO_SR_48K);
  sample_rate = DAISY.get_samplerate();

  limiter.Init();
  flanger.Init(sample_rate);
  phaser.Init(sample_rate);

  tremolo.Init(sample_rate);
  tremolo.SetFreq(7.21);
  tremolo.SetDepth(0.8);

  autowah.Init(sample_rate);
  autowah.SetLevel(1.0);
  autowah.SetWah(0.5);

  compressor.Init(sample_rate);

  //hihat.Init(sample_rate);
  //bassdrum.Init(sample_rate);
  //snaredrum.Init(sample_rate);  

  button1.Init(1000, true, D0, INPUT);
  button2.Init(1000, true, D1, INPUT);
  button3.Init(1000, true, D2, INPUT);

  DAISY.begin(AudioCallback);
}

void loop() {
  knob6 = analogRead(A5) / 1023.f;
  knob5 = analogRead(A4) / 1023.f;
  knob4 = analogRead(A3) / 1023.f;
  knob3 = analogRead(A2) / 1023.f;
  knob2 = analogRead(A1) / 1023.f;
  knob1 = analogRead(A0) / 1023.f;

  button1.Debounce();
  button2.Debounce();
  button3.Debounce();
}

float fuzz(float input, float level) {
  if (level <= 0.0) {
    return input;
  }
  float output = input * (1.0 + (level * 16.0));
  if (output > 0.5) {
    output = 0.5;
  }
  if (output < -0.5) {
    output = -0.5;
  }
  return output;
}

