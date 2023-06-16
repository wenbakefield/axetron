# AxeTron: Embedded Effects for Electric Guitar
![knobs](https://github.com/wenbakefield/axetron/assets/8831999/c61031a0-303b-4aae-ba1c-65b123981d4d)

AxeTron represents a new generation of electric guitars, combining traditional and novel design elements for enhanced portability and expanded control during live performances.

Check out the [paper](https://github.com/wenbakefield/axetron/blob/52b90f5a8087c217043038534a7bf72ba9d7f503/axetron_report.pdf)!

## Technical Specifications

- **Integrated System:** AxeTron is an all-in-one system, reducing reliance on external gear. It integrates embedded DSP hardware for a broad range of audio effects.

- **Control Interface:** The guitar is equipped with a comprehensive control interface, including knobs, buttons, and touchpads, for real-time, granular control of audio effects.

- **Preset Management:** It supports multiple effect presets, which can be customized and switched dynamically by the player to meet specific performance needs.

- **Advanced Features:** It includes looping and drum machine effects, expanding the sound possibilities beyond a traditional guitar, especially for solo musicians.

- **Built-in Theremin:** A built-in theremin synth module provides a further layer of tone variety, allowing for unique and diverse sound output.

- **Custom Firmware:** Users can upload their custom Arduino/C++ firmware to the device, gaining complete control over functionality and performance.

## Daisy Seed

The heart of AxeTron's technical prowess is the Daisy Seed microcontroller. This robust, high-performance hardware is used in the Arduino development environment and is strategically embedded within the guitar. Daisy Seed, known for its high audio quality, low latency, and ample computing power, makes it the perfect choice for our multifunctional electric guitar.

The provided Arduino code sets the foundation for the AxeTron guitar's multi-effects system. It leverages the DaisyDuino library to interface with the Daisy Seed microcontroller. Additionally, it incorporates the Trill library to read data from the Trill touch sensors present on the guitar.

### Effects
Effects are mapped to various knobs on the guitar. Each effect is processed only if the respective knob's value is above a small threshold (0.01 in this code). The knobs' values are read using the analogRead() function, and these readings are normalized between 0 and 1. Here's a brief rundown of the supported effects and how they are controlled:

-    **Fuzz:** Controlled by `knob1`. The fuzz effect is a type of distortion that changes the waveform into a square wave, creating a buzzy tone. The intensity of the fuzz effect depends on the level parameter.

-    **Flanger:** Controlled by `knob2`. The Flanger effect uses an LFO (Low Frequency Oscillator) to create a sweeping effect. The depth of the LFO is controlled by `knob2`.

-    **Compressor:** Controlled by `knob4`. A compressor reduces the dynamic range of the audio signal. The threshold of the compressor is controlled by `knob4`.

-    **Tremolo:** Controlled by `trillX` and `trillY` (from Trill touch sensors). Tremolo modulates the volume of the signal at a given frequency. The frequency and depth of the tremolo effect are controlled by the touch sensor readings.

-    **Bitcrusher (Decimator):** Controlled by `knob5`. A Bitcrusher reduces the bitrate of the audio signal, creating a lo-fi effect. The bitcrush factor is controlled by `knob5`.

-    **Autowah:** Controlled by `knob6`. An Autowah is an envelope filter that changes the frequency response based on the input volume. The dry/wet mix of the effect is controlled by `knob6`.

### Program Structure

The `setup()` function initializes the Daisy Seed with a sample rate of 48kHz and initializes the various audio effect objects. It also sets initial parameters for the effects and initializes the Trill touch sensor and a debounce switch for a button.

The `loop()` function is the heart of the program. It constantly reads the knobs' values, updates the Trill touch sensor readings, and checks for a button press.

The `AudioCallback()` function is where the audio processing takes place. For each audio sample, the function applies the various effects in sequence if their respective knobs are above the threshold. The processed audio sample is then sent to both audio output channels.
