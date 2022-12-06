# ToneFileGenerator
A simple C++ CLI application for generating a sine wave tone.
## Specs
Creates an audio wave file with pure sinusoidal sound.
on input: sound frequency in Hertz and a length of the file in seconds.
on output:
1. audio.wav file 24 bit, mono, 48Khz in current directory. OR
2. an exception with an explanation if there was an error anywhere.