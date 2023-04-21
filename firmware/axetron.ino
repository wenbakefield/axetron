
// Title: AxeTron
// Description: Onboard multieffects for electric guitar
// Hardware: Daisy Seed
// Author: Ben Wakefield

#include "DaisyDuino.h"
#include <Trill.h>
#include <Wire.h>

DaisyHardware hw;
Trill trill;

Flanger flanger;
Tremolo tremolo;
Autowah autowah;
Compressor compressor;
//ReverbSc reverb;
Decimator decimator;
//Looper looper;

float knob1;
float knob2;
float knob3;
float knob4;
float knob5;
float knob6;

float trillX;
float trillY;

Switch button1trig;
int button1 = -1;

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
    // if (knob3 > 0.01) {
    //   phaser.SetLfoDepth(knob3);
    //   output_sample = phaser.Process(output_sample);
    // }

    if (knob4 > 0.01) {
      compressor.SetThreshold(knob4 * -80.0);
      output_sample = compressor.Process(output_sample);
    }

    if (trillX > 0.06 || trillY > 0.06) {
      tremolo.SetFreq(trillX * 10.0);
      tremolo.SetDepth(trillY);
      output_sample = tremolo.Process(output_sample);
    }

    if (knob5 > 0.01) {
      decimator.SetBitcrushFactor(knob5);
      output_sample = decimator.Process(output_sample);
    }
    if (knob6 > 0.01) {
      autowah.SetDryWet(knob6 * 100);
      output_sample = autowah.Process(output_sample);
    }

    //output_sample = output_sample * (knob3 / 2.0);


    out[0][i] = out[1][i] = output_sample;
  }
}

void setup() {
  float sample_rate;
  hw = DAISY.init(DAISY_SEED, AUDIO_SR_48K);
  sample_rate = DAISY.get_samplerate();

  flanger.Init(sample_rate);

  tremolo.Init(sample_rate);
  tremolo.SetFreq(7.21);
  tremolo.SetDepth(0.8);

  autowah.Init(sample_rate);
  autowah.SetLevel(1.0);
  autowah.SetWah(0.5);

  decimator.Init();

  compressor.Init(sample_rate);

  button1trig.Init(1000, false, D0, INPUT);

  Wire.begin();
  trill.setup(Trill::TRILL_SQUARE, 0x28);

  //pinMode(A6, OUTPUT);

  DAISY.begin(AudioCallback);
}

void loop() {
  knob6 = analogRead(A5) / 1023.f;
  knob5 = analogRead(A4) / 1023.f;
  knob4 = analogRead(A3) / 1023.f;
  knob3 = analogRead(A2) / 1023.f;
  knob2 = analogRead(A1) / 1023.f;
  knob1 = analogRead(A0) / 1023.f;

  button1trig.Debounce();

  trill.read();
  float positionX = trill.touchHorizontalLocation(0);
  float positionY = trill.touchLocation(0);
  if (positionX >= 0) {
    trillX = 1.0 - (positionX / 1800.0);
  }
  if (positionY >= 0) {
    trillY = 1.0 - (positionY / 1800.0);
  }

  if (button1trig.RisingEdge()) {
    button1 = button1 * -1;
  }

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

