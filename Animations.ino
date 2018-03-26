
/********************************************************************
 * SimplePatterns:
 * These are static patterns that rely on a method like shift to
 * move them around one step at a time. As to not busy the
 * arduino from checking for sensor input.
 * 
 * These all need a no param method that has default values
 *******************************************************************/

void meteor() {
  meteorChaser(15, 12, 160, false);
}

void meteorChaser(uint16_t meteorBodyPixel, uint8_t tailLength, uint16_t fadeValue, bool rainbowTail) {
  uint16_t i;
  int fadeSpectrum = fadeValue;
  int fadeIncrement = (256 - fadeValue) / tailLength;

  if (rainbowTail == true) {
    fill_rainbow(&(strip[meteorBodyPixel]), tailLength, hue);
  } else {
    if (animationDirection == true) {
      for (i = meteorBodyPixel; i > (meteorBodyPixel - tailLength); i--) {
        strip[i].setHue(hue);
        strip[i].fadeLightBy(fadeSpectrum);
        fadeSpectrum += fadeIncrement;
      }
    } else {
      for (i = (meteorBodyPixel - tailLength); i < meteorBodyPixel; i++) {
        strip[i].setHue(hue);
        strip[i].fadeLightBy(fadeSpectrum);
        fadeSpectrum += fadeIncrement;
      }
    }
  }
  FastLED.show();
}


//wtf do I even name this?
uint8_t correctNumber(int number) {
  if (number > NUM_PIXELS - 1)
    return (number - NUM_PIXELS);
  else if (number < 0)
    return (number + NUM_PIXELS);
  else
    return number;
}

void wipeRainbow() {
  fill_rainbow(&(strip[0]), NUM_PIXELS, 0);
}

void fourPoints() {
  fourPoints(0, 20, 40, 60);
}

void fourPoints(uint8_t point1, uint8_t point2, uint8_t point3, uint8_t point4) {
  for (uint8_t i = 0; i < NUM_PIXELS; i++) {
    if (i == point1 || i == point2 || i == point3 || i == point4) {
      strip[i].setHue(hue);
    }
  }
  FastLED.show();
}

void cycleSolid() {
  hue++;
  for (int i = 0; i < NUM_PIXELS; i++) {
    strip[i].setHue(hue);
  }
}

//do multiple like it says, dick.
void blinkRandom(uint8_t numberRandomPixels, bool randomColor) {
  uint8_t randomPixel = random8(60);
  uint8_t randomHue = random8();
  for (int i = 0; i < NUM_PIXELS; i++) {
    strip[i] = CRGB::Black;
    if(randomPixel == i) {
      if(randomColor == true) {
        strip[i].setHue(hue);
      } else {
        strip[i].setHue(randomHue);
      }
    }
  }
}

// random colored speckles that blink in and fade smoothly
void confetti() {
    fadeToBlackBy(strip, NUM_PIXELS, 25);
    uint8_t pos = random16(NUM_PIXELS);
    strip[pos] += CHSV(hue + random8(64), 200, 200);

    EVERY_N_MILLISECONDS(20)
    { hue++; }
}

// colored stripes pulsing at a defined Beats-Per-Minute (BPM)
void bpm() {
    uint8_t BeatsPerMinute = 120;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
    for (int i = 0; i < NUM_PIXELS; i++) { //9948
        strip[i] = ColorFromPalette(palette, hue + (i * 2), beat - hue + (i * 8));
    }
    EVERY_N_MILLISECONDS(20)
    { hue++; } // slowly cycle the "base color" through the rainbow
}

// a colored dot sweeping back and forth, with fading trails
void sinelon() {
    fadeToBlackBy(strip, NUM_PIXELS, 10);
    int pos = beatsin16(13, 0, NUM_PIXELS);
    strip[pos] += CHSV(hue, 200, 192);

    EVERY_N_MILLISECONDS(20)
    { hue++; } // slowly cycle the "base color" through the rainbow
}


void juggle() {
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy(strip, NUM_PIXELS, 20);
    byte dothue = 0;
    for (int i = 0; i < 6; i++) {
        strip[beatsin16(i + 5, 0, NUM_PIXELS)] |= CHSV(dothue, 200, 180);
        dothue += 32;
    }

    EVERY_N_MILLISECONDS(20)
    { hue++; } // slowly cycle the "base color" through the rainbow
}
