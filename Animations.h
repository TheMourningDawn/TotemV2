#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "TimerOne.h"
#include "Torus.h"
#include <Adafruit_CircuitPlayground.h>

#define BINS   10          // FFT output is filtered down to this many bins
#define FRAMES 4           // This many FFT cycles are averaged for leveling

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

class Animations {
private:
  Torus *totem;
  uint8_t hue;
  uint8_t currentIndex; //Used for keeping track of a moving index
  uint8_t tempCounter; //Secondary track keeping int for whatever
  boolean animationDirection;
  uint16_t spectrum[BINS]; // FFT spectrum output buffer

  uint8_t lvl[FRAMES][BINS]; // Bin levels for the prior #FRAMES frames
  uint8_t avgLo[BINS];       // Pseudo rolling averages for bins -- lower and
  uint8_t avgHi[BINS];       // upper limits -- for dynamic level adjustment.
  uint8_t frameIdx = 0;      // Counter for lvl storage
public:
  Animations();
  Animations(Torus *totem);
  
  //Long running animations
  void wipeSolidFromBottom();
  
  void meteor();
  void meteorChaser(uint16_t meteorBodyPixel, uint8_t tailLength, uint16_t fadeValue, bool rainbowTail);
  uint8_t correctNumber(int number);
  void wipeRainbow();
  void fourPoints();
  void fourPoints(uint8_t point1, uint8_t point2, uint8_t point3, uint8_t point4);
  void blinkRandom(uint8_t numberRandomPixels, bool randomColor);
  void confetti();
  void bpm();
  void sinelon();
  void juggle();

  //testing
  void wipeInfinity();
  void testFFT();
  void runFFT();
  
  void wipeRandom();
  
  //Movement animations
  void cycle();
};

#endif
