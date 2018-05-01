#ifndef TORUS_H
#define TORUS_H

#include <FastLED.h>
#include "Utils.h"

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
    uint8_t fade;
    uint8_t brightness;
    uint8_t saturation;
    uint8_t animationSpeed;
    boolean direction;
public:
    Torus();
    Torus(CRGB *ledStrip, uint8_t bottomIndex);
    CRGB *getStrip();

    //Set and fill operations
    void setPixel(uint8_t index);
    void setPixel(uint8_t index, CRGB color);
    void setPixel(uint8_t index, CHSV color);
    void setPixel(uint8_t index, uint8_t hue);
    void setRightPixel(CRGB color);
    void setLeftPixel(CRGB color);
    void setTopPixel(CRGB color);
    void setBottomPixel(CRGB color);
    void clearStrip();
    void fill(int fromIndex, int toIndex, CRGB color);
    void fill(int fromIndex, int toIndex, uint8_t color);

    // Wipe are the same as fill, except they animate the fill.
    // of course this blocks until the loop is done. YYMV.
    void wipe(int fromIndex, int toIndex, CRGB color);
    void wipe(int fromIndex, int toIndex, uint8_t color);

    void shiftClockwise(int shiftFromPixel);
    void shiftCounterClockwise(int shiftFromPixel);

    //Modifications
    void fadePixel(uint8_t index);
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
    boolean getDirection();
    void changeDirection();

    uint8_t getHue();
    void setHue(uint8_t newHue);
    void incrementHue(uint8_t amount);

    uint8_t getFade();
    void setFade(uint8_t fadeValue);

    uint8_t getBrightness();
    void setBrightness(uint8_t newBrightness);

    uint8_t getSaturation();
    void setSaturation(uint8_t newSatuation);

    uint8_t getAnimationSpeed();
    void setAnimationSpeed(uint8_t newSpeed);
};

#endif
