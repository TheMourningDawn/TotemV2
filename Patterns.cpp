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

void Patterns::meteorRainbow() {
    fill_rainbow(&(totem->getStrip()[0]), totem->getFade(), totem->getHue());
}

void Patterns::rainbow() {
    fill_rainbow(&(totem->getStrip()[0]), totem->length(), totem->getHue());
}

// Four points equally spaced, moving in the same directio
void Patterns::fourPoints() {
    for (uint8_t i = 0; i < totem->length(); i++) {
        if (i == totem->getBottomPixelIndex() ||
            i == totem->getRightPixelIndex() ||
            i == totem->getLeftPixelIndex() ||
            i == totem->getTopPixelIndex()) {
            totem->setPixel(i, totem->getHue());
        }
    }
}

void Patterns::halfTopBottom() {
    CRGB topColor = CHSV(totem->getHue(), totem->getSaturation(), totem->getBrightness());
    CRGB bottomColor = CHSV(totem->getHue() + 100, totem->getSaturation(), totem->getBrightness());
    halfTopBottom(true, topColor, bottomColor);
}

void Patterns::halfTopBottom(bool animate, CRGB colorTop, CRGB colorBottom) {
    uint8_t leftIndex = totem->getLeftPixelIndex();
    uint8_t rightIndex = totem->getRightPixelIndex();

    totem->fill(leftIndex, rightIndex, colorTop);
    totem->fill(rightIndex, leftIndex, colorBottom);
}

void Patterns::fourSquare() {
    totem->fill(totem->getBottomPixelIndex(), totem->getRightPixelIndex(), CHSV(Utils::wrap(totem->getHue() + 100, 255), totem->getSaturation(), totem->getBrightness()));
    totem->fill(totem->getRightPixelIndex(), totem->getTopPixelIndex(), CHSV(Utils::wrap(totem->getHue() + 190, 255), totem->getSaturation(), totem->getBrightness()));
    totem->fill(totem->getTopPixelIndex(), totem->getLeftPixelIndex(), CHSV(Utils::wrap(totem->getHue() + 90, 255), totem->getSaturation(), totem->getBrightness()));
    totem->fill(totem->getLeftPixelIndex(), totem->getBottomPixelIndex(), CHSV(Utils::wrap(totem->getHue() + 140, 255),totem->getSaturation(), totem->getBrightness()));
}

#endif
