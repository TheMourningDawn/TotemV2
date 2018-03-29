#ifndef TORUS_H
#define TORUS_H

#include <FastLED.h>

FASTLED_USING_NAMESPACE;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

class Torus {
private:
  CRGB *strip;
  uint8_t stripLength;
  uint8_t leftIndex;
  uint8_t rightIndex;
  uint8_t topIndex;
  uint8_t bottomIndex;
  uint8_t hue;
  boolean direction;
public:
  Torus();
  Torus(CRGB *ledStrip, uint8_t bottomIndex);
  CRGB* getStrip();

  //Set and fill operations
  void setPixel(uint8_t index, CRGB color);
  void setPixel(uint8_t index, uint8_t hue);
  void setRightPixel(CRGB color);
  void setLeftPixel(CRGB color);
  void setTopPixel(CRGB color);
  void setBottomPixel(CRGB color);
  void clearStrip();

  //TODO: Make wipe and fill methdods

  //Modifications
  void fadePixel(uint8_t index, int fade);

  //Retrieval
  uint8_t getRightPixelIndex();
  uint8_t getLeftPixelIndex();
  uint8_t getTopPixelIndex();
  uint8_t getBottomPixelIndex();
  CRGB getPixel(uint8_t index);
  CRGB getRightPixel();
  CRGB getLeftPixel();
  CRGB getTopPixel();
  CRGB getBottomPixel();
  uint8_t length();

  //Manipulations
  void cycle();
  void cycleClockwise();
  void cycleCounterClockwise();

  //Global
  void changeDirection();
  boolean getDirection();
  uint8_t getHue();
  void setHue(uint8_t newHue);
  void incrementHue(uint8_t amount);
};

#endif
