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

  //Transition only animations
  void cycle();
};

#endif
