#ifndef ANIMATIONS_CPP
#define ANIMATIONS_CPP

#include "Animations.h"

Animations::Animations() {}

Animations::Animations(Torus *totem, Equalizer *equalizer) : totem(totem), equalizer(equalizer) {
    pendulum = new Pendulum(totem);
    animationDirection = true;
    hue = 0;
    currentIndex = 0;
    tempCounter = 0;
}


/********************************************************
 * Short animations
 * 
 * These animations only do one step of the animation
 * at one time. This is preferable, because it does
 * not block the board from receiving input from
 * the knob or updates from the accel, etc
 */

void Animations::cycle() {
    totem->cycle();
}

void Animations::meteor() {
    fadeToBlackBy(totem->getStrip(), totem->length(), totem->getFade());
    if (totem->getDirection() == true) {
        currentIndex = Utils::wrap(currentIndex + 1, totem->length() - 1);
    } else {
        currentIndex = Utils::wrap(currentIndex - 1, totem->length() - 1);
    }
    totem->setPixel(currentIndex, totem->getHue());
}

void Animations::blinkRandom() {
    uint8_t randomPixel = random8(totem->length() - 1);
    uint8_t randomHue = random8();
    for (int i = 0; i < totem->length(); i++) {
        totem->setPixel(i, CRGB(0, 0, 0));
        if (randomPixel == i) {
            totem->setPixel(i, CHSV(randomHue, totem->getSaturation(), totem->getBrightness()));
        }
    }
}

// random colored speckles that blink in and fade smoothly
void Animations::confetti() {
    fadeToBlackBy(totem->getStrip(), totem->length(), 25);
    uint8_t pos = random16(totem->length());
    totem->setPixel(pos, CHSV(totem->getHue() + random8(64), totem->getSaturation(), totem->getHue()));

    EVERY_N_MILLISECONDS(20)
    { totem->incrementHue(1); }
}

// colored stripes pulsing at a defined Beats-Per-Minute (BPM)
void Animations::bpm() {
    uint8_t BeatsPerMinute = 120;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8(BeatsPerMinute, 20, 255);
    for (int i = 0; i < totem->length(); i++) { //9948
        totem->setPixel(i, ColorFromPalette(palette, totem->getHue() + (i * 2), beat - hue + (i * 8)));
    }

    EVERY_N_MILLISECONDS(20)
    { totem->incrementHue(1); }
}

// a colored dot sweeping back and forth, with fading trails
void Animations::sinelon() {
    fadeToBlackBy(totem->getStrip(), totem->length(), 10);
    int pos = beatsin16(13, 0, totem->length() - 1);
    totem->setPixel(pos, CHSV(totem->getHue(), totem->getSaturation(), totem->getBrightness()));

    EVERY_N_MILLISECONDS(20)
    { totem->incrementHue(1); }
}

// eight colored dots, weaving in and out of sync with each other
void Animations::juggle() {
    fadeToBlackBy(totem->getStrip(), totem->length(), 20);
    byte dothue = 0;
    for (int i = 0; i < 6; i++) {
        totem->getStrip()[beatsin16(i + 5, 0, totem->length() - 1)] |= CHSV(dothue, totem->getSaturation(), totem->getHue());
        dothue += 32;
    }

    EVERY_N_MILLISECONDS(20)
    { totem->incrementHue(1); }
}

//Solid color changing at the bottom every rotation
void Animations::wipeSolidFromBottom() {
    if(currentIndex == 0) {
        totem->incrementHue(random8(15, 100));
    }
    totem->setPixel(currentIndex, totem->getHue());
    if(totem->getDirection() == true) {
        currentIndex = Utils::wrap(currentIndex+1, totem->length()-1);
    } else  {
        currentIndex = Utils::wrap(currentIndex-1, totem->length()-1);
    }
}

void Animations::wipeRainbow() {
    totem->setPixel(currentIndex, totem->getHue());
    if(totem->getDirection() == true) {
        currentIndex = Utils::wrap(currentIndex+1, totem->length()-1);
    } else  {
        currentIndex = Utils::wrap(currentIndex-1, totem->length()-1);
    }
    totem->incrementHue(4);
}

void Animations::wipeRandom() {
    totem->setPixel(currentIndex, CHSV(totem->getHue(), totem->getSaturation(), totem->getHue()));
    if (totem->getDirection() == true) {
        currentIndex = Utils::wrap(currentIndex+1, totem->length()-1);
    } else {
        currentIndex = Utils::wrap(currentIndex-1, totem->length()-1);
    }
    //What I want is to change the color at some random point after it's gone 1/4 of the circle
    //the only thing I can think of is oddly complicated and maybe inconsistant?
    tempCounter++;
    if (tempCounter > totem->length() / 4 + random8(0, totem->length() / 2)) {
        totem->incrementHue(random8(20, 100));
        tempCounter = 0;
    }
}


/********************************************************
 * Long running animations
 * 
 * These animations do loops inside of the functions,
 * so they end up blocking the arduino from being
 * able to change modes or accept other input
 * for a period of time
 */




/********************************************************
 * Testing animations
 *
 * Trying new things!
 */

void Animations::simonSaysDropTheBase() {
    equalizer->readAudioFrequencies();

    if (equalizer->getBand(0) > 0) {
        totem->fill(totem->getBottomPixelIndex(), totem->getRightPixelIndex(), random(255));
    } else {
        totem->fill(totem->getBottomPixelIndex(), totem->getRightPixelIndex(), CRGB(0, 0, 0));
    }
    if (equalizer->getBand(1) > 0) {
        totem->fill(totem->getRightPixelIndex(), totem->getTopPixelIndex(), random(255));
    } else {
        totem->fill(totem->getRightPixelIndex(), totem->getTopPixelIndex(), CRGB(0, 0, 0));
    }
    if (equalizer->getBand(2) > 0) {
        totem->fill(totem->getTopPixelIndex(), totem->getLeftPixelIndex(), random(255));
    } else {
        totem->fill(totem->getTopPixelIndex(), totem->getLeftPixelIndex(), CRGB(0, 0, 0));
    }
    if (equalizer->getBand(3) > 0) {
        totem->fill(totem->getLeftPixelIndex(), totem->length() - 1, random(255));
    } else {
        totem->fill(totem->getLeftPixelIndex(), totem->length() - 1, CRGB(0, 0, 0));
    }
}

void Animations::waterfallEqualizer() {
    equalizer->readAudioFrequencies();
    int spectrumz = 1;
    int threshold = 140;
    int offset = 120;
    if (equalizer->frequencies[spectrumz] > threshold) {
        int color = map(equalizer->frequencies[spectrumz], threshold, 1024, 0, 255);
        Utils::wrap(color + offset, 255);
        totem->setPixel(totem->getTopPixelIndex(), color);
        totem->getStrip()[totem->getTopPixelIndex()].nscale8_video(color);
    } else {
        totem->setPixel(totem->getTopPixelIndex(), CRGB(0, 0, 0));
    }
    totem->shiftClockwise(totem->getTopPixelIndex());
    totem->shiftCounterClockwise(totem->getTopPixelIndex());
}

int8_t i = 0;
int8_t j = 0;
void Animations::hemiola() {
    EVERY_N_MILLISECONDS((8 / 8) * 20)
    {
        i = Utils::wrap(i+1, totem->length() - 1);
        totem->setPixel(i, CHSV(totem->getHue(), totem->getSaturation(), totem->getBrightness()));
        totem->setPixel(Utils::wrap(i - 1, totem->length() - 1), CRGB(0, 0, 0));
        FastLED.show();
    }
    EVERY_N_MILLISECONDS((3 / 4) * 20)
    {
        j = Utils::wrap(j-1, totem->length() - 1);
        totem->setPixel(j, CHSV(totem->getHue() + 120, totem->getSaturation(), totem->getBrightness()));
        totem->setPixel(Utils::wrap(j - 1, totem->length() - 1), CRGB(0, 0, 0));
        FastLED.show();
    }
}

void Animations::pendulumSinglePoint() {
    pendulumMode(CHSV(totem->getHue(), 255, 255), true);
}

void Animations::pendulumAntiGravity() {
    pendulumMode(CHSV(totem->getHue(), 255, 255), false);
}

void Animations::pendulumMode(CRGB color, bool antiGravity) {
    fadeToBlackBy(totem->getStrip(), totem->length(), totem->getFade());

    pendulum->readAccelerometerData();

    totem->setPixel(pendulum->getPosition());
}


#endif
