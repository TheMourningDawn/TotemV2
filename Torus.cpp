#ifndef TORUS_CPP
#define TORUS_CPP

#include "Torus.h"

Torus::Torus() {}

Torus::Torus(CRGB *ledStrip, uint8_t bottomIndex) : strip(ledStrip) {
  direction = false;
  stripLength = 80;
  hue = 0;
  fade = 20;
  brightness = 255;
  saturation = 255;
  bottomIndex = bottomIndex;
  topIndex = (stripLength/2 + bottomIndex) % stripLength;
  leftIndex = (stripLength/4*3 + bottomIndex) % stripLength;
  rightIndex = (stripLength/4 + bottomIndex) % stripLength;
}

CRGB* Torus::getStrip() {
  return strip;
}

uint8_t Torus::getRightPixelIndex() {
   return rightIndex;
}

uint8_t Torus::getLeftPixelIndex() {
    return leftIndex;
}

uint8_t Torus::getTopPixelIndex() {
  return topIndex;
}

uint8_t Torus::getBottomPixelIndex() {
  return bottomIndex;
}

void Torus::clearStrip() {
  fill_solid( &(strip[0]), length(), CRGB::Black);
}

/*
 * Sets pixel at index with current
 * hue/saturation/brightness global values
 */
void Torus::setPixel(uint8_t index) {
  strip[index] = CHSV(hue, saturation, brightness);
}

void Torus::setPixel(uint8_t index, CRGB color) {
  strip[index] = color;
}

void Torus::setPixel(uint8_t index, uint8_t hue) {
    strip[index] = CHSV(hue, saturation, brightness);
}

void Torus::setRightPixel(CRGB color) {
  strip[rightIndex] = color;
}

void Torus::setLeftPixel(CRGB color) {
  strip[leftIndex] = color;
}

void Torus::setBottomPixel(CRGB color) {
  strip[bottomIndex] = color;
}

void Torus::setTopPixel(CRGB color) {
  strip[topIndex] = color;
}

void Torus::fill(int fromIndex, int toIndex, CRGB color) {
  if(toIndex < fromIndex) {
    //We're jumping the gap at the end...just split it and recurse
    fill(0, toIndex, color);
    toIndex = length()-1;
  }
  int numberToFill = abs(toIndex - fromIndex) + 1;
  int startFillFrom = fromIndex;
  fill_solid( &(strip[startFillFrom]), numberToFill, color);
}

void Torus::fill(int fromIndex, int toIndex, uint8_t color) {
  Serial.print("Filling from: ");
  Serial.print(fromIndex);
  Serial.print(" , Filling to: ");
  Serial.println(toIndex);
  if(toIndex < fromIndex) {
    //We're jumping the gap at the end...just split it and recurse
    fill(0, toIndex, color);
    toIndex = length()-1;
  }
  int numberToFill = abs(toIndex - fromIndex) +  1;
  int startFillFrom = fromIndex;
  fill_solid( &(strip[startFillFrom]), numberToFill, CHSV(color, saturation, brightness));
}

void wipe(int fromIndex, int toIndex, CRGB color) {

}
void wipe(int fromIndex, int toIndex, uint8_t color) {

}

void Torus::fadePixel(uint8_t index) {
  strip[index].fadeLightBy(fade);
}

void Torus::fadePixel(uint8_t index, int fade) {
 strip[index].fadeLightBy(fade);
}

CRGB Torus::getPixel(uint8_t index) {
  return strip[index];
}

CRGB Torus::getRightPixel() {
  return strip[rightIndex];
}

CRGB Torus::getLeftPixel() {
  return strip[leftIndex];
}

CRGB Torus::getBottomPixel() {
  return strip[bottomIndex];
}

CRGB Torus::getTopPixel() {
  return strip[topIndex];
}

uint8_t Torus::length() {
  return stripLength;
}

void Torus::cycleClockwise() {
  CRGB firstLed = strip[0];
  memmove(&strip[0], &strip[1], (length()-1) * sizeof(CRGB));
  strip[length()-1] = firstLed;
}

void Torus::cycleCounterClockwise() {
  CRGB lastLed = strip[length()-1];
  memmove(&strip[1], &strip[0], (length()-1) * sizeof(CRGB));
  strip[0] = lastLed;
}

void Torus::cycle() {
  if(direction == true) {
    cycleClockwise();
  } else {
    cycleCounterClockwise();
  }
}

void Torus::changeDirection() {
  if(direction == true) {
    direction = false;
  } else {
    direction = true;
  }
}

boolean Torus::getDirection() {
  return direction;
}

uint8_t Torus::getHue() {
  return hue;
}

void Torus::setHue(uint8_t newHue) {
  hue = newHue;
}

void Torus::incrementHue(uint8_t amount) {
  hue += amount;
}

uint8_t Torus::getFade() {
    return fade;
}

void Torus::setFade(uint8_t fadeValue) {
    fade = fadeValue;
}

uint8_t Torus::getBrightness() {
  return brightness;
}

void Torus::setBrightness(uint8_t newBrightness) {
  brightness = newBrightness;
}

uint8_t Torus::getSaturation() {
  return saturation;
}
void Torus::setSaturation(uint8_t newSaturation) {
  saturation = newSaturation;
}

void Torus::shiftClockwise(int shiftFromPixel) {
  int distance = (length()-1) - shiftFromPixel;
  memmove(&strip[shiftFromPixel+1], &strip[shiftFromPixel], (distance) * sizeof(CRGB));
}

void Torus::shiftCounterClockwise(int shiftFromPixel) {
  int distance = shiftFromPixel;
  memmove(&strip[0], &strip[1], (distance) * sizeof(CRGB));
}

#endif
