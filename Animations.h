#ifndef ANIMATIONS_H
#define ANIMATIONS_H

//#include "TimerOne.h"
#include "Torus.h"
#include "Equalizer.h"
#include "Utils.h"
#include <Adafruit_CircuitPlayground.h>

#define BINS   10          // FFT output is filtered down to this many bins
#define FRAMES 4           // This many FFT cycles are averaged for leveling

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

class Animations {
private:
    Torus *totem;
    Equalizer *equalizer;
    uint8_t hue;
    uint8_t currentIndex; //Used for keeping track of a moving index
    uint8_t tempCounter; //Secondary track keeping int for whatever
    boolean animationDirection;

    //All pendulum related fields. Really I think we should just move this into another classsssssss
    float pos;  // Starting center position of pupil
    float increment; // distance between pixels in radians
    float MomentumH; // horizontal component of pupil rotational inertia
    float MomentumV; // vertical component of pupil rotational inertia

// Tuning constants. (a.k.a. "Fudge Factors)
// These can be tweaked to adjust the liveliness and sensitivity of the eyes.
    const float friction = 0.985; // frictional damping constant.  1.0 is no friction.
    const float swing = 20;  // arbitrary divisor for gravitational force
    const float gravity = 100;  // arbitrary divisor for lateral acceleration
    const float gestureThreshold = -0.80; // accelerometer threshold for toggling modes
    const float pupilRadius = 2; // half-width of pupil (in pixels)
public:
    Animations();
    Animations(Torus *totem, Equalizer *equalizer);

    //Long running animations
    void wipeSolidFromBottom();

    //Non-blocking animations
    void meteor();
    void wipeRainbow();
    void fourPoints();
    void fourPoints(uint8_t point1, uint8_t point2, uint8_t point3, uint8_t point4);
    void blinkRandom();
    void confetti();
    void bpm();
    void sinelon();
    void juggle();
    void hemiola();

    //testing
    void simonSaysDropTheBase();
    void wipeInfinity();
    void wipeRandom();
    void waterfallEqualizer();

    void pendulum();
    void pendulumAntiGravity();
    void pendulumMirrored();
    void pendulumMode(CRGB color, bool antiGravity, bool mirroredEyes);

    //Transition only animations
    void cycle();
};

#endif
