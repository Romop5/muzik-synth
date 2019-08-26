# Muzik-Synth

## What is it ? 

A bunch of *C++11 and higher templates*, created for simple musical synthesis.

## Usage

A custom instrument / sample can be constructed using types available in `src/lib/muzik.hpp` and `muzik` namespace
by composing template arguments of predefined classes.

For example, a simple sin at frequency 5, modulating basic sine at frequency 50 can be expressed as:

```cpp

using instrument = muzik::Sin<F(5), muzik::Sin<F(50)>>;
size_t seconds = 10;
for(size_t sample = 0; sample < seconds*muzik::SAMPLING_RATE; sample++)
{
    auto amplitude = instrument.value(sample);
    ...
}

```

## Why
Exploiting possibilities for making my own demo once I learn enough.