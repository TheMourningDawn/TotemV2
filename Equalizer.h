#ifndef EQUALIZER_H
#define EQUALIZER_H

#import <Arduino.h>

#define STROBE_PIN 2
#define RESET_PIN 3
#define EQ_PIN A10

class Equalizer {
public:
    int frequencies[7];
    Equalizer();
    void readAudioFrequencies();
};

#endif
