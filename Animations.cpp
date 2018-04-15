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
  meteorChaser(15, 12, 160, false);
}

void Animations::meteorChaser(uint16_t meteorBodyPixel, uint8_t tailLength, uint16_t fadeValue, bool rainbowTail) {
  uint16_t i;
  int fadeSpectrum = fadeValue;
  int fadeIncrement = (256 - fadeValue) / tailLength;

  if (rainbowTail == true) {
    fill_rainbow(&(totem->getStrip()[meteorBodyPixel]), tailLength, hue);
  } else {
    if (animationDirection == true) {
      for (i = meteorBodyPixel; i > (meteorBodyPixel - tailLength); i--) {
        totem->setPixel(i, hue);
        totem->fadePixel(i, fadeSpectrum);
        fadeSpectrum += fadeIncrement;
      }
    } else {
      for (i = (meteorBodyPixel - tailLength); i < meteorBodyPixel; i++) {
        totem->setPixel(i, hue);
        totem->fadePixel(i, fadeSpectrum);
        fadeSpectrum += fadeIncrement;
      }
    }
  }
}

void Animations::wipeRainbow() {
  fill_rainbow(&(totem->getStrip()[0]), totem->length()-1, 0);
}

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

//do multiple like it says, dick.
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

int8_t i = 0;
int8_t j = 0;
void Animations::hemiola() {
    EVERY_N_MILLISECONDS((8/8)*20) { 
      i++;
      i = clamp(i);
      totem->setPixel(i, CHSV(totem->getHue(), 200, 192));
      totem->setPixel(clamp(i-1), CRGB(0, 0, 0));
      FastLED.show();
    }
    EVERY_N_MILLISECONDS((3/4)*20) { 
      j++; 
      j = clamp(j);
      totem->setPixel(j, CHSV(totem->getHue()+120, 200, 192));
      totem->setPixel(clamp(j-1), CRGB(0, 0, 0));
      FastLED.show();
    }
}

int8_t Animations::clamp(int8_t index) {
  if (index > totem->length() - 1)
    return (index - totem->length());
  else if (index < 0)
    return (index + totem->length());
  else
    return index;
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
    clamp(color+offset, 255);
    totem->setPixel(39, color);
    totem->getStrip()[39].nscale8_video(color);
  } else {
    totem->setPixel(39, CRGB(0,0,0));
  }
  totem->shiftClockwise(39);
  totem->shiftCounterClockwise(39);
}

int Animations::clamp(int value, int maxValue) {
  if (value < 0) {
    return value + maxValue;
  }
  if (value > maxValue - 1) {
    return value - maxValue;
  }
  return value;
}

#endif
