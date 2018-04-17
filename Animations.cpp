#ifndef ANIMATIONS_CPP
#define ANIMATIONS_CPP

#include "Animations.h"

Animations::Animations() {}

Animations::Animations(Torus *totem, Equalizer *equalizer) : totem(totem), equalizer(equalizer) {
  animationDirection = true;
  hue = 0;
  currentIndex = 0;
  tempCounter = 0;
  uint16_t spectrum[BINS];
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
    currentIndex = Utils::wrap(currentIndex+1, totem->length()-1);
    totem->setPixel(currentIndex, totem->getHue());
}

void Animations::wipeRainbow() {
  fill_rainbow(&(totem->getStrip()[0]), totem->length()-1, 0);
}


//TODO: consolodate this to just one function using top/bottom/right/left indexes.
// dont see much point in making this one configurable in any way
void Animations::fourPoints() {
  fourPoints(0, 20, 40, 60);
}

void Animations::fourPoints(uint8_t point1, uint8_t point2, uint8_t point3, uint8_t point4) {
  for (uint8_t i = 0; i < totem->length(); i++) {
    if (i == point1 || i == point2 || i == point3 || i == point4) {
      totem->setPixel(i, hue);
    }
  }
}

//TODO: Use this to see what is lol
void Animations::blinkRandom(uint8_t numberRandomPixels, bool randomColor) {
  uint8_t randomPixel = random8(60);
  uint8_t randomHue = random8();
  for (int i = 0; i < totem->length(); i++) {
    totem->setPixel(i, CRGB::Black);
    if(randomPixel == i) {
      if(randomColor == true) {
        totem->setPixel(i, hue);
      } else {
        totem->setPixel(i, randomHue);
      }
    }
  }
}

// random colored speckles that blink in and fade smoothly
void Animations::confetti() {
    fadeToBlackBy(totem->getStrip(), totem->length(), 25);
    uint8_t pos = random16(totem->length());
    totem->setPixel(pos, CHSV(totem->getHue() + random8(64), 200, 200));

    EVERY_N_MILLISECONDS(20) { totem->incrementHue(1); }
}

// colored stripes pulsing at a defined Beats-Per-Minute (BPM)
void Animations::bpm() {
    uint8_t BeatsPerMinute = 120;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
    for (int i = 0; i < totem->length(); i++) { //9948
      totem->setPixel(i, ColorFromPalette(palette, totem->getHue() + (i * 2), beat - hue + (i * 8)));
    }
  
    EVERY_N_MILLISECONDS(20) { totem->incrementHue(1); }
}

// a colored dot sweeping back and forth, with fading trails
void Animations::sinelon() {
    fadeToBlackBy(totem->getStrip(), totem->length(), 10);
    int pos = beatsin16(13, 0, totem->length()-1);
    totem->setPixel(pos, CHSV(totem->getHue(), 200, 192));

    EVERY_N_MILLISECONDS(20) { totem->incrementHue(1); }
}

void Animations::juggle() {
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy(totem->getStrip(), totem->length(), 20);
    byte dothue = 0;
    for (int i = 0; i < 6; i++) {
      totem->getStrip()[beatsin16(i + 5, 0, totem->length()-1)] |= CHSV(dothue, 200, 180);
      dothue += 32;
    }

    EVERY_N_MILLISECONDS(20) { totem->incrementHue(1); }
}


/********************************************************
 * Long running animations
 * 
 * These animations do loops inside of the functions,
 * so they end up blocking the arduino from being
 * able to change modes or accept other input
 * for a period of time
 */

//TODO: Put in the animation speed, yo
void Animations::wipeSolidFromBottom() {
  uint8_t i;
  if (totem->getDirection() == true) {
    for (i = 0; i < totem->length()-1; i++) {
      totem->setPixel(i, totem->getHue());
      FastLED.show();
      delay(20);
    }
  } else {
    for (i = totem->length()-1; i >= 1; --i) {
      totem->setPixel(i - 1, totem->getHue());
      FastLED.show();
      delay(20);
    }
  }
  totem->incrementHue(random8(100));
}


/********************************************************
 * Testing animations
 *
 * Trying new things!
 */

void Animations::wipeInfinity() {
  for (uint8_t i = 0; i < totem->length()-1; i++) {
    totem->setPixel(i, totem->getHue());
    FastLED.show();
    delay(20);
    totem->incrementHue(4);
  }
}


void Animations::wipeRandom() {
  totem->setPixel(currentIndex, CHSV(totem->getHue(), 255, 255));
  if(totem->getDirection() == true) {
    currentIndex++;
    if(currentIndex > totem->length()-1) {
      currentIndex = 0;
    }
  } else {
    currentIndex--;
    if(currentIndex > totem->length()-1) {
      currentIndex = totem->length()-1;
    }
  }
  //What I want is to change the color at some random point after it's gone 1/4 of the circle
  //the only thing I can think of is oddly complicated and maybe inconsistant?
  tempCounter++;
  if(tempCounter > totem->length()/4 + random8(0, totem->length()/2)) {
    totem->incrementHue(random8(20, 60));
    tempCounter = 0;
  }
}

void Animations::waterfallEqualizer() {
  equalizer->readAudioFrequencies();
  int spectrumz = 1;
  int threshold = 140;
  int offset = 120;
  if(equalizer->frequencies[spectrumz] > threshold) {
    int color = map(equalizer->frequencies[spectrumz], threshold, 1024, 0, 255);
    Utils::wrap(color+offset, 255);
    totem->setPixel(39, color);
    totem->getStrip()[39].nscale8_video(color);
  } else {
    totem->setPixel(39, CRGB(0,0,0));
  }
  totem->shiftClockwise(39);
  totem->shiftCounterClockwise(39);
}

int8_t i = 0;
int8_t j = 0;
void Animations::hemiola() {
    EVERY_N_MILLISECONDS((8/8)*20) {
        i++;
        i = Utils::wrap(i, totem->length()-1);
        totem->setPixel(i, CHSV(totem->getHue(), 200, 192));
        totem->setPixel(Utils::wrap(i-1, totem->length()-1), CRGB(0, 0, 0));
        FastLED.show();
    }
    EVERY_N_MILLISECONDS((3/4)*20) {
        j++;
        j = Utils::wrap(j, totem->length()-1);
        totem->setPixel(j, CHSV(totem->getHue()+120, 200, 192));
        totem->setPixel(Utils::wrap(j-1, totem->length()-1), CRGB(0, 0, 0));
        FastLED.show();
    }
}

#endif
