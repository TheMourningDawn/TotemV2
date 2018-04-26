#include <Wire.h>
#include "TimerOne.h"
#include "ClickEncoder.h"
#include "Torus.h"
#include "Animations.h"
#include "Patterns.h"
#include "Equalizer.h"
#include <Adafruit_CircuitPlayground.h>

#define STRIP_PIN 12
#define SETTINGS_STRIP_PIN 17

#define PIN_ENCODER_A 0
#define PIN_ENCODER_B 1
#define PIN_ENCODER_SWITCH 6

#define STROBE_PIN 2
#define RESET_PIN 3
#define EQ_PIN A10

#define NUM_PIXELS 80
#define NUM_SETTING_PIXELS 10

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

ClickEncoder *encoder;
int16_t previousEncoderValue = 0;
int16_t currentEncoderValue = 0;

CRGB strip[NUM_PIXELS];
CRGB settings_strip[NUM_SETTING_PIXELS];

int frequencies[7];

Torus *totem;
Patterns *patterns;
Animations *animations;
Equalizer *equalizer;

uint8_t currentMode = 0;
uint8_t currentPattern = 0;
int animationSpeed = 10;

void timerIsr() {
    encoder->service();
}

// For some reason all these typedefs have to be below a void fuction
// God only knows why
typedef void (*Mode)();

typedef Mode ModeList[];
const ModeList modes = {
        patternSelectMode,
        patternSpeedMode,
        patternColorMode,
        patternFadeMode,
        patternBrightnessMode,
        patternSaturationMode,
        sensitivitySelectMode
};

typedef void (Patterns::*Pattern)();

typedef void (Animations::*Animation)();

typedef struct {
    Pattern pattern;
    Animation animation;
} PatternDefinition;
typedef PatternDefinition PatternDefinitionList[];

//TODO: Get a better name, yo
const PatternDefinitionList pattern_list = {
        {&Patterns::nothing,       &Animations::meteor},
        {&Patterns::nothing,       &Animations::blinkRandom},
        {&Patterns::nothing,       &Animations::pendulum},
//        {&Patterns::whatever,      &Animations::cycle}, //TODO: Something wrong with this, causing resets
        {&Patterns::meteor,        &Animations::cycle},
        {&Patterns::fourPoints,    &Animations::cycle},
        {&Patterns::nothing,       &Animations::bpm},
        {&Patterns::nothing,       &Animations::juggle},
        {&Patterns::nothing,       &Animations::sinelon},
        {&Patterns::nothing,       &Animations::confetti},
        {&Patterns::nothing,       &Animations::wipeSolidFromBottom},
        {&Patterns::nothing,       &Animations::wipeRainbow},
        {&Patterns::nothing,       &Animations::wipeInfinity},
        {&Patterns::halfTopBottom, &Animations::cycle},
        {&Patterns::nothing,       &Animations::wipeRandom},
        {&Patterns::nothing,       &Animations::hemiola},
        {&Patterns::nothing,       &Animations::waterfallEqualizer},
        {&Patterns::nothing,       &Animations::simonSaysDropTheBase},
};

void setup() {
    Serial.begin(9600);

    // Initialize the circuit playground board
    CircuitPlayground.begin();

    //Initialize FastLED for the main strip and the settings strip
    FastLED.addLeds<NEOPIXEL, STRIP_PIN>(strip, NUM_PIXELS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<NEOPIXEL, SETTINGS_STRIP_PIN>(settings_strip, NUM_SETTING_PIXELS).setCorrection(TypicalLEDStrip);

    totem = new Torus(strip, 0);
    patterns = new Patterns(totem);
    equalizer = new Equalizer();
    animations = new Animations(totem, equalizer);

    //Initialize the encoder (knob) and it's interrupt
    encoder = new ClickEncoder(0, 1, 6, 4, false);
    Timer1.initialize(1000);
    Timer1.attachInterrupt(timerIsr);

    // Display the initial pattern
    displaySettingMode();
    (patterns->*pattern_list[currentPattern].pattern)();
}

void loop() {
    checkEncoderInput();
    (animations->*pattern_list[currentPattern].animation)();
    FastLED.show();
    delay(animationSpeed);
}

/*
 * Checks for click or rotation input from the rotary encoder
 * 
 * Click: Changes mode selection
 * Double click: 
 * Long click: Change direction of animation
 * Rotate: Change value
 */
void checkEncoderInput() {
    currentEncoderValue += encoder->getValue();

    ClickEncoder::Button b = encoder->getButton();
    if (b != ClickEncoder::Open) {
        switch (b) {
            case ClickEncoder::Clicked:
//                Serial.println("Button clicked");
                cycleSettingsMode();
                break;
            case ClickEncoder::DoubleClicked:
//                Serial.println("Button double clicked");
                encoder->setAccelerationEnabled(!encoder->getAccelerationEnabled());
                break;
            case ClickEncoder::Released:
//                Serial.println("Button held");
                totem->changeDirection();
                break;
            default:;
        }
    }

    if (b == ClickEncoder::Open) {
        if (currentEncoderValue != previousEncoderValue) {
            modes[currentMode]();
            previousEncoderValue = currentEncoderValue;
        }
    }
}

void patternSelectMode() {
    if (currentEncoderValue > previousEncoderValue) {
        nextPattern();
    } else if (currentEncoderValue < previousEncoderValue) {
        previousPattern();
    }
}

void patternSpeedMode() {
    if (currentEncoderValue > previousEncoderValue) {
        animationSpeed -= 2;
    } else if (currentEncoderValue < previousEncoderValue) {
        animationSpeed += 2;
    }
    if (animationSpeed < 2) {
        animationSpeed = 0;
    }
//    Serial.print("Speed: ");
//    Serial.println(animationSpeed);
}

void patternColorMode() {
    totem->setHue(totem->getHue() + 3);
    totem->clearStrip();
    (patterns->*pattern_list[currentPattern].pattern)();
//    Serial.print("Hue: ");
//    Serial.println(totem->getHue());
}

void patternFadeMode() {
    if (currentEncoderValue > previousEncoderValue) {
        totem->setFade(Utils::clamp(totem->getFade() + 1, 100));
    } else if (currentEncoderValue < previousEncoderValue) {
        totem->setFade(Utils::clamp(totem->getFade() - 1, 100));
    }
//    Serial.print("Fade: ");
//    Serial.println(totem->getFade());
}

void patternBrightnessMode() {
    if (currentEncoderValue > previousEncoderValue) {
        totem->setBrightness(totem->getBrightness() + 1);
    } else if (currentEncoderValue < previousEncoderValue) {
        totem->setBrightness(totem->getBrightness() - 1);
    }
    // TODO: This is a bit of a hack so the pattern will update. Dunno if I should be doing this all the time.
    totem->clearStrip();
    (patterns->*pattern_list[currentPattern].pattern)();

//    Serial.print("Brightness: ");
//    Serial.println(totem->getBrightness());
}

void patternSaturationMode() {
    if (currentEncoderValue > previousEncoderValue) {
        totem->setSaturation(totem->getSaturation() + 1);
    } else if (currentEncoderValue < previousEncoderValue) {
        totem->setSaturation(totem->getSaturation() - 1);
    }
    // TODO: This is a bit of a hack so the pattern will update. Dunno if I should be doing this all the time.
    totem->clearStrip();
    (patterns->*pattern_list[currentPattern].pattern)();

//    Serial.print("Saturation: ");
//    Serial.println(totem->getSaturation());
}

void sensitivitySelectMode() {
    if (currentEncoderValue > previousEncoderValue) {
        equalizer->setSensitivity(Utils::clamp(equalizer->getSensitivity() + 20, 1024));
    } else if (currentEncoderValue < previousEncoderValue) {
        equalizer->setSensitivity(Utils::clamp(equalizer->getSensitivity() - 20, 1024));
    }

    // TODO: This is a bit of a hack so the pattern will update. Dunno if I should be doing this all the time.
    totem->clearStrip();
    (patterns->*pattern_list[currentPattern].pattern)();

//    Serial.print("Sensitivity: ");
//    Serial.println(equalizer->getSensitivity());
}

void frequencySelectMode() {
    if (currentEncoderValue > previousEncoderValue) {
        equalizer->setFrequencyOffset(Utils::clamp(equalizer->getFrequencyOffset() + 1, 7));
    } else if (currentEncoderValue < previousEncoderValue) {
        equalizer->setSensitivity(Utils::clamp(equalizer->getSensitivity() - 1, 7));
    }

//    Serial.print("Frequency: ");
//    Serial.println(equalizer->getSensitivity());
}

void nextPattern() {
    currentPattern = Utils::wrap(currentPattern + 1, ARRAY_SIZE(pattern_list) - 1);
//    Serial.print("Next pattern: ");
//    Serial.println(currentPattern);
    totem->clearStrip();
    (patterns->*pattern_list[currentPattern].pattern)();
}

void previousPattern() {
    currentPattern = Utils::wrap(currentPattern - 1, ARRAY_SIZE(pattern_list) - 1);
//    Serial.print("Previous pattern: ");
//    Serial.println(currentPattern);
    totem->clearStrip();
    (patterns->*pattern_list[currentPattern].pattern)();
}

