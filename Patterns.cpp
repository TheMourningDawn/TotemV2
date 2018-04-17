/********************************************************************
 * SimplePatterns:
 * These are static patterns that rely on an animation like cycle
 * to move them around one step at a time. As to not busy the
 * arduino from checking for sensor input.
 *
 * These all need a no param method that has default values
 *******************************************************************/
#ifndef PATTERNS_CPP
#define PATTERNS_CPP

#include "Patterns.h"

Patterns::Patterns() {}

Patterns::Patterns(Torus *totem) : totem(totem) {}

void Patterns::nothing() {
  //literally
}

void Patterns::meteor() {
  meteorChaser(30, 25, 50, false);
}

void Patterns::meteorRainbow() {
  meteorChaser(20, 25, 50, true);
}

void Patterns::meteorChaser(uint16_t meteorBodyPixel, uint8_t tailLength, uint16_t fadeValue, bool rainbowTail) {
  uint16_t i;
  int fadeSpectrum = fadeValue;
  int fadeIncrement = (256 - fadeValue) / tailLength;

  if (rainbowTail == true) {
    fill_rainbow(&(totem->getStrip()[meteorBodyPixel]), tailLength, totem->getHue());
  } else {
    if (totem->getDirection() == true) {
      for (i = meteorBodyPixel; i > (meteorBodyPixel - tailLength); i--) {
        totem->setPixel(i, totem->getHue());
        totem->fadePixel(i, fadeSpectrum);
        fadeSpectrum += fadeIncrement;
      }
    } else {
      for (i = (meteorBodyPixel - tailLength); i < meteorBodyPixel; i++) {
        totem->setPixel(i, totem->getHue());
        totem->fadePixel(i, fadeSpectrum);
        fadeSpectrum += fadeIncrement;
      }
    }
  }
}

void Patterns::wipeRainbow() {
  fill_rainbow(&(totem->getStrip()[0]), totem->length(), 0);
}

void Patterns::fourPoints() {
  fourPoints(0, 20, 40, 60);
}

void Patterns::fourPoints(uint8_t point1, uint8_t point2, uint8_t point3, uint8_t point4) {
  for (uint8_t i = 0; i < totem->length(); i++) {
    if (i == point1 || i == point2 || i == point3 || i == point4) {
      totem->setPixel(i, totem->getHue());
    }
  }
  FastLED.show();
}

void Patterns::halfTopBottom() {
  CRGB topColor = CHSV(totem->getHue(), 255, 255);
  CRGB bottomColor = CHSV(totem->getHue() + 100, 255, 255);
  halfTopBottom(true, topColor, bottomColor);
}

void Patterns::halfTopBottom(bool animate, CRGB colorTop, CRGB colorBottom) {
  uint8_t leftIndex = totem->getLeftPixelIndex();
  uint8_t rightIndex = totem->getRightPixelIndex();

  totem->fill(leftIndex, rightIndex, colorTop);
  totem->fill(rightIndex, leftIndex, colorBottom);
}

void Patterns::whatever() {
  totem->fill(totem->getBottomPixelIndex(), totem->getRightPixelIndex(), CHSV(totem->getHue()+100, 255, 255));
  totem->fill(totem->getRightPixelIndex(), totem->getTopPixelIndex(), CHSV(totem->getHue()+30, 255, 255));
  totem->fill(totem->getTopPixelIndex(), totem->getLeftPixelIndex(), CHSV(totem->getHue()+90, 255, 255));
  totem->fill(totem->getLeftPixelIndex(), totem->getBottomPixelIndex(), CHSV(totem->getHue()+140, 255, 255));
}

#endif
