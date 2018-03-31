#ifndef ANIMATIONS_CPP
#define ANIMATIONS_CPP

#include "Animations.h"

const uint8_t PROGMEM
  // Low-level noise initially subtracted from each of 32 FFT bins
  noise[]    = { 0x04,0x03,0x03,0x03, 0x02,0x02,0x02,0x02,
                 0x02,0x02,0x02,0x02, 0x01,0x01,0x01,0x01,
                 0x01,0x01,0x01,0x01, 0x01,0x01,0x01,0x01,
                 0x01,0x01,0x01,0x01, 0x01,0x01,0x01,0x01 },
  // FFT bins (32) are then filtered down to 10 output bins (to match the
  // number of NeoPixels on Circuit Playground).  10 arrays here, one per
  // output bin.  First element of each is the number of input bins to
  // merge, second element is index of first merged bin, remaining values
  // are scaling weights as each input bin is merged into output.  The
  // merging also "de-linearizes" the FFT output, so it's closer to a
  // logarithmic scale with octaves evenly-ish spaced, music looks better.
  bin0data[] = { 1, 2, 147 },
  bin1data[] = { 2, 2, 89, 14 },
  bin2data[] = { 2, 3, 89, 14 },
  bin3data[] = { 4, 3, 15, 181, 58, 3 },
  bin4data[] = { 4, 4, 15, 181, 58, 3 },
  bin5data[] = { 6, 5, 6, 89, 185, 85, 14, 2 },
  bin6data[] = { 7, 7, 5, 60, 173, 147, 49, 9, 1 },
  bin7data[] = { 10, 8, 3, 23, 89, 170, 176, 109, 45, 14, 4, 1 },
  bin8data[] = { 13, 11, 2, 12, 45, 106, 167, 184, 147, 89, 43, 18, 6, 2, 1 },
  bin9data[] = { 18, 14, 2, 6, 19, 46, 89, 138, 175, 185, 165, 127, 85, 51, 27, 14, 7, 3, 2, 1 },
  // Pointers to 10 bin arrays, because PROGMEM arrays-of-arrays are weird:
  * const binData[] = { bin0data, bin1data, bin2data, bin3data, bin4data,
                        bin5data, bin6data, bin7data, bin8data, bin9data },
  // R,G,B values for color wheel covering 10 NeoPixels:
  reds[]   = { 0xAD, 0x9A, 0x84, 0x65, 0x00, 0x00, 0x00, 0x00, 0x65, 0x84 },
  greens[] = { 0x00, 0x66, 0x87, 0x9E, 0xB1, 0x87, 0x66, 0x00, 0x00, 0x00 },
  blues[]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xE4, 0xFF, 0xE4, 0xC3 },
  gamma8[] = { // Gamma correction improves the appearance of midrange colors
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x06,
    0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09,
    0x0A, 0x0A, 0x0A, 0x0B, 0x0B, 0x0B, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x0E,
    0x0E, 0x0F, 0x0F, 0x10, 0x10, 0x11, 0x11, 0x12, 0x12, 0x13, 0x13, 0x14,
    0x14, 0x15, 0x15, 0x16, 0x16, 0x17, 0x18, 0x18, 0x19, 0x19, 0x1A, 0x1B,
    0x1B, 0x1C, 0x1D, 0x1D, 0x1E, 0x1F, 0x1F, 0x20, 0x21, 0x22, 0x22, 0x23,
    0x24, 0x25, 0x26, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2A, 0x2B, 0x2C, 0x2D,
    0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x44, 0x45, 0x46,
    0x47, 0x48, 0x49, 0x4B, 0x4C, 0x4D, 0x4E, 0x50, 0x51, 0x52, 0x54, 0x55,
    0x56, 0x58, 0x59, 0x5A, 0x5C, 0x5D, 0x5E, 0x60, 0x61, 0x63, 0x64, 0x66,
    0x67, 0x69, 0x6A, 0x6C, 0x6D, 0x6F, 0x70, 0x72, 0x73, 0x75, 0x77, 0x78,
    0x7A, 0x7C, 0x7D, 0x7F, 0x81, 0x82, 0x84, 0x86, 0x88, 0x89, 0x8B, 0x8D,
    0x8F, 0x91, 0x92, 0x94, 0x96, 0x98, 0x9A, 0x9C, 0x9E, 0xA0, 0xA2, 0xA4,
    0xA6, 0xA8, 0xAA, 0xAC, 0xAE, 0xB0, 0xB2, 0xB4, 0xB6, 0xB8, 0xBA, 0xBC,
    0xBF, 0xC1, 0xC3, 0xC5, 0xC7, 0xCA, 0xCC, 0xCE, 0xD1, 0xD3, 0xD5, 0xD7,
    0xDA, 0xDC, 0xDF, 0xE1, 0xE3, 0xE6, 0xE8, 0xEB, 0xED, 0xF0, 0xF2, 0xF5,
    0xF7, 0xFA, 0xFC, 0xFF };
const uint16_t PROGMEM
  // Scaling values applied to each FFT bin (32) after noise subtraction
  // but prior to merging/filtering.  When multiplied by these values,
  // then divided by 256, these tend to produce outputs in the 0-255
  // range (VERY VERY "ISH") at normal listening levels.  These were
  // determined empirically by throwing lots of sample audio at it.
  binMul[] = { 405, 508, 486, 544, 533, 487, 519, 410,
               481, 413, 419, 410, 397, 424, 412, 411,
               511, 591, 588, 577, 554, 529, 524, 570,
               546, 559, 511, 552, 439, 488, 483, 547, },
  // Sums of bin weights for bin-merging tables above.
  binDiv[]   = { 147, 103, 103, 257, 257, 381, 444, 634, 822, 1142 };

Animations::Animations() {}

Animations::Animations(Torus *totem) : totem(totem) {
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

void Animations::runFFT() {
  CircuitPlayground.mic.fft(spectrum);

  // spectrum[] is now raw FFT output, 32 bins.

  // Remove noise and apply EQ levels
  uint8_t  i, N;
  uint16_t S;
  for(i=0; i<32; i++) {
    N = pgm_read_byte(&noise[i]);
    if(spectrum[i] > N) { // Above noise threshold: scale & clip
      S           = ((spectrum[i] - N) *
                     (uint32_t)pgm_read_word(&binMul[i])) >> 8;
      spectrum[i] = (S < 255) ? S : 255;
    } else { // Below noise threshold: clip
      spectrum[i] = 0;
    }
  }
  // spectrum[] is now noise-filtered, scaled & clipped
  // FFT output, in range 0-255, still 32 bins.

  // Filter spectrum[] from 32 elements down to 10,
  // make pretty colors out of it:

  uint16_t sum, level;
  uint8_t  j, minLvl, maxLvl, nBins, binNum, *data;

  uint8_t realMVP[10];

  for(i=0; i<BINS; i++) { // For each output bin (and each pixel)...
    data   = (uint8_t *)pgm_read_word(&binData[i]);
    nBins  = pgm_read_byte(&data[0]); // Number of input bins to merge
    binNum = pgm_read_byte(&data[1]); // Index of first input bin
    data  += 2;
    for(sum=0, j=0; j<nBins; j++) {
      sum += spectrum[binNum++] * pgm_read_byte(&data[j]); // Total
    }
    sum /= pgm_read_word(&binDiv[i]);                      // Average
    Serial.println(sum);
    lvl[frameIdx][i] = sum;      // Save for rolling averages
    minLvl = maxLvl = lvl[0][i]; // Get min and max range for bin
    for(j=1; j<FRAMES; j++) {    // from prior stored frames
      if(lvl[j][i] < minLvl)      minLvl = lvl[j][i];
      else if(lvl[j][i] > maxLvl) maxLvl = lvl[j][i];
    }
    // minLvl and maxLvl indicate the extents of the FFT output for this
    // bin over the past few frames, used for vertically scaling the output
    // graph (so it looks interesting regardless of volume level).  If too
    // close together though (e.g. at very low volume levels) the graph
    // becomes super coarse and 'jumpy'...so keep some minimum distance
    // between them (also lets the graph go to zero when no sound playing):
    if((maxLvl - minLvl) < 23) {
      maxLvl = (minLvl < (255-23)) ? minLvl + 23 : 255;
    }
    avgLo[i] = (avgLo[i] * 7 + minLvl) / 8; // Dampen min/max levels
    avgHi[i] = (maxLvl >= avgHi[i]) ?       // (fake rolling averages)
      (avgHi[i] *  3 + maxLvl) /  4 :       // Fast rise
      (avgHi[i] * 31 + maxLvl) / 32;        // Slow decay

    // Second fixed-point scale then 'stretches' each bin based on
    // dynamic min/max levels to 0-256 range:
    level = 1 + ((sum <= avgLo[i]) ? 0 :
                 256L * (sum - avgLo[i]) / (long)(avgHi[i] - avgLo[i]));
    spectrum[i] = level;
  }
}

void Animations::testFFT() {
  runFFT();
  int spectrumz = 1;
  int base_offset = 100;
  if(spectrum[spectrumz] > 20) {
      totem->setPixel(39, spectrum[5]+base_offset);
  } else {
    totem->setPixel(39, CRGB(0,0,0));
  }
  totem->shiftClockwise(39);
  totem->shiftCounterClockwise(39);
}

#endif
