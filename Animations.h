#ifndef ANIMATIONS_H
#define ANIMATIONS_H

//#include "TimerOne.h"
#include "Torus.h"
#include "Equalizer.h"
#include "Pendulum.h"
#include "Utils.h"
#include <Adafruit_CircuitPlayground.h>

#define BINS   10          // FFT output is filtered down to this many bins
#define FRAMES 4           // This many FFT cycles are averaged for leveling

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

class Animations {
private:
    Torus *totem;
    Equalizer *equalizer;
    Pendulum *pendulum;
    uint8_t hue;
    uint8_t currentIndex; //Used for keeping track of a moving index
    uint8_t tempCounter; //Secondary track keeping int for whatever
    boolean animationDirection;
public:
    Animations();
    Animations(Torus *totem, Equalizer *equalizer);

    //Long running animations
    void wipeSolidFromBottom();

    //Non-blocking animations
    void meteor();
    void wipeRainbow();
    void wipeRandom();
    void blinkRandom();
    void confetti();
    void bpm();
    void sinelon();
    void juggle();
    void hemiola();

    //Sound reactive
    void waterfallEqualizer();
    void simonSaysDropTheBase();

    //testing
    void pendulumSinglePoint();
    void pendulumAntiGravity();
    void pendulumMirrored();
    void pendulumMode(CRGB color, bool antiGravity);

    //Transition only animations
    void cycle();
};

#endif
