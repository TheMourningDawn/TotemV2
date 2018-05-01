#ifndef EQUALIZER_H
#define EQUALIZER_H

#include <Arduino.h>
#include "Utils.h"

#define STROBE_PIN 2
#define RESET_PIN 3
#define EQ_PIN A10

class Equalizer {
private:
    uint16_t sensitivity;
    uint8_t frequencyOffset;
public:
    Equalizer();
    int frequencies[7];
    void readAudioFrequencies();
    uint8_t getBand(uint8_t band);
    uint8_t getBand(uint8_t band, uint16_t sensitivityThreshold);
    int getFrequencyOffset();
    void setFrequencyOffset(int newFrequencyOffset);
    uint16_t getSensitivity();
    void setSensitivity(uint16_t newSensitivity);
    void resetFrequencyOffset();
};

#endif
