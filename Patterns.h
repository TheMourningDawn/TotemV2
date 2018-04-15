#ifndef PATTERNS_H
#define PATTERNS_H

#include "TimerOne.h"
#include "Torus.h"

FASTLED_USING_NAMESPACE;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

class Patterns {
private:
  Torus *totem;
public:
  Patterns();
  Patterns(Torus *totem);
  void nothing();
  void meteor();
  void meteorRainbow();
  void meteorChaser(uint16_t meteorBodyPixel, uint8_t tailLength, uint16_t fadeValue, bool rainbowTail);
  void wipeRainbow();
  void fourPoints();
  void fourPoints(uint8_t point1, uint8_t point2, uint8_t point3, uint8_t point4);
  void halfTopBottom();
  void halfTopBottom(bool animate, CRGB colorTop, CRGB colorBottom);
  void whatever();

};

#endif
