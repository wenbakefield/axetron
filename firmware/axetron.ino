// Title: AxeTron
// Description: Onboard multieffects for electric guitar
// Hardware: Daisy Seed
// Author: Ben Wakefield

#include "DaisyDuino.h"

DaisyHardware hw;

Limiter limiter;
Flanger flanger;
Phaser phaser;
Chorus chorus;
Tremolo tremolo;
Autowah autowah;
PitchShifter pitchshifter;
Compressor compressor;
ReverbSc reverb;


float knob1;
float knob2;
float knob3;
float knob4;
float knob5;
float knob6;
float knob7;
float knob8;


void AudioCallback(float **in, float **out, size_t size) {
  flanger.SetLfoDepth(knob2);
  phaser.SetLfoDepth(knob3);
  chorus.SetLfoDepth(knob4);
  tremolo.SetDepth(knob5);
  autowah.SetDryWet(knob6);
  pitchshifter.SetTransposition((knob7 * 24.f) - 12.f);
  compressor.SetThreshold(knob8);

  for (size_t i = 0; i < size; i++) {
    float input_sample = in[0][i] * 8;
    float output_sample = input_sample;
    if (knob1 > 0.01) {
      output_sample = fuzz(output_sample, knob1);
    }
    if (knob2 > 0.01) {
      output_sample = flanger.Process(output_sample);
    }
    if (knob3 > 0.01) {
      output_sample = phaser.Process(output_sample);
    }
    if (knob4 > 0.01) {
      output_sample = chorus.Process(output_sample);
    }
    if (knob5 > 0.01) {
      output_sample = tremolo.Process(output_sample);
    }
    if (knob6 > 0.01) {
      output_sample = autowah.Process(output_sample);
    }
    if (knob7 > 0.01) {
      output_sample = pitchshifter.Process(output_sample);
    }
    if (knob8 > 0.01) {
      output_sample = compressor.Process(output_sample);
    }
    // reverb.Process(output_sample, output_sample, output_sample, output_sample);

    out[0][i] = out[1][i] = output_sample;
  }
}

void setup() {
  float sample_rate;
  hw = DAISY.init(DAISY_SEED, AUDIO_SR_48K);
  sample_rate = DAISY.get_samplerate();

  limiter.Init();

  flanger.Init(sample_rate);
  phaser.Init(sample_rate);
  chorus.Init(sample_rate);
  tremolo.Init(sample_rate);
  autowah.Init(sample_rate);
  pitchshifter.Init(sample_rate);
  compressor.Init(sample_rate);

  DAISY.begin(AudioCallback);
}

void loop() {
  knob1 = analogRead(A7) / 1023.f;
  knob2 = analogRead(A6) / 1023.f;
  knob3 = analogRead(A5) / 1023.f;
  knob4 = analogRead(A4) / 1023.f;
  knob5 = analogRead(A3) / 1023.f;
  knob6 = analogRead(A2) / 1023.f;
  knob7 = analogRead(A1) / 1023.f;
  knob8 = analogRead(A0) / 1023.f;
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

