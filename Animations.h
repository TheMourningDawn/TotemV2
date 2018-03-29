#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "TimerOne.h"
#include "Torus.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

class Animations {
private:
  Torus *totem;
  uint8_t hue;
  uint8_t currentIndex; //Used for keeping track of a moving index
  uint8_t tempCounter; //Secondary track keeping int for whatever
  boolean animationDirection;
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

  void wipeInfinity();
  
  void wipeRandom();
  
  //Movement animations
  void cycle();
};

#endif
