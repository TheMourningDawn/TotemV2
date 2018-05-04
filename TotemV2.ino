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
CRGB settingsStrip[NUM_SETTING_PIXELS];

int frequencies[7];

Torus *totem;
Patterns *patterns;
Animations *animations;
Equalizer *equalizer;

uint8_t currentMode = 0;
uint8_t currentPattern = 0;
bool delayAnimation = true;

void timerIsr() {
    encoder->service();
}

// For some reason all these typedefs have to be below a void function, God only knows why.
typedef void (*Mode)();

typedef Mode ModeList[];
const ModeList modes = {
        patternSelectMode,
        patternSpeedMode,
        patternColorMode,
        patternBrightnessMode,
        patternSaturationMode,
        patternFadeMode,
        sensitivitySelectMode,
        frequencySelectMode
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
        {&Patterns::nothing,       &Animations::meteorRainbow},
        {&Patterns::nothing,       &Animations::middleFanout},
        {&Patterns::nothing,       &Animations::crazyMiddleFanout},
        {&Patterns::nothing,       &Animations::wipeRainbow},
        {&Patterns::nothing,       &Animations::rainbowPush},
        {&Patterns::rainbow,       &Animations::cycle},
        {&Patterns::nothing,       &Animations::wipeSolidFromBottom},
        {&Patterns::nothing,       &Animations::wipeRandom},
        {&Patterns::nothing,       &Animations::blinkRandom},
        {&Patterns::nothing,       &Animations::confetti},
        {&Patterns::nothing,       &Animations::bpm},
        {&Patterns::nothing,       &Animations::juggle},
        {&Patterns::nothing,       &Animations::sinelon},
        {&Patterns::nothing,       &Animations::hemiola},

        {&Patterns::fourPoints,    &Animations::cycle},
        {&Patterns::halfTopBottom, &Animations::cycle},
        {&Patterns::fourSquare,    &Animations::cycle},

        {&Patterns::nothing,       &Animations::pendulumSinglePoint},

        {&Patterns::nothing,       &Animations::waterfallEqualizer},
        {&Patterns::nothing,       &Animations::simonSaysDropTheBase},
};

void setup() {
    Serial.begin(9600);

    // Initialize the circuit playground board
    CircuitPlayground.begin();

    //Initialize FastLED for the main strip and the settings strip
    FastLED.addLeds<NEOPIXEL, STRIP_PIN>(strip, NUM_PIXELS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<NEOPIXEL, SETTINGS_STRIP_PIN>(settingsStrip, NUM_SETTING_PIXELS).setCorrection(TypicalLEDStrip);

    totem = new Torus(strip, 78);
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
    modes[currentMode]();
    (animations->*pattern_list[currentPattern].animation)();
    FastLED.show();
    if (delayAnimation == true) {
        delay(totem->getAnimationSpeed());
    }
}

/*
 * Checks for click or rotation input from the rotary encoder
 * 
 * Click: Change mode selection
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
                cycleSettingsMode();
                break;
            case ClickEncoder::DoubleClicked:
                delayAnimation = !delayAnimation;
                break;
            case ClickEncoder::Released:
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
        spinSettingsClockwise();
        nextPattern();
    } else if (currentEncoderValue < previousEncoderValue) {
        spinSettingsCounterClockwise();
        previousPattern();
    }
}

int settingSpeed = 2;
int flux = 0;
void patternSpeedMode() {
    if (currentEncoderValue > previousEncoderValue) {
        totem->setAnimationSpeed(totem->getAnimationSpeed() - 2);
        if(totem->getAnimationSpeed() <= 0) {
            flashSettingsRight(CRGB::Red);
        }
    } else if (currentEncoderValue < previousEncoderValue) {
        totem->setAnimationSpeed(totem->getAnimationSpeed() + 2);
        if(totem->getAnimationSpeed() >= 120) {
            flashSettingsLeft(CRGB::Red);
        }
    }

    flux = beatsin16(120, 0, 4096);
    if(flux >= 4096) {
        settingSpeed++;
        if(settingSpeed > 15) {
            settingSpeed = 0;
        }
    }
    if(flux % settingSpeed == 0) {
        spinSettingsClockwise();
    }
//    Serial.print("Speed: ");
//    Serial.println(animationSpeed);
}

void patternColorMode() {
    if (currentEncoderValue > previousEncoderValue) {
        totem->setHue(totem->getHue() + 2);
    } else if (currentEncoderValue < previousEncoderValue) {
        totem->setHue(totem->getHue() - 2);
    }

    if (currentEncoderValue != previousEncoderValue) {
        settingsStrip[2].setHue(totem->getHue());
        totem->clearStrip();
        (patterns->*pattern_list[currentPattern].pattern)();
    }
    fill_solid( &settingsStrip[0], 10, CHSV(beatsin8(5, 0, 255), 255, 255));
}

void patternFadeMode() {
    if (currentEncoderValue > previousEncoderValue) {
        totem->setFade(Utils::clamp(totem->getFade() + 1, 79));
        if(totem->getFade() >= 79) {
            flashSettingsRight(CRGB::Red);
        }
    } else if (currentEncoderValue < previousEncoderValue) {
        totem->setFade(Utils::clamp(totem->getFade() - 1, 79));
        if(totem->getFade() <= 0) {
            flashSettingsLeft(CRGB::Red);
        }
    }
    fadeToBlackBy(settingsStrip, 10, 20);
    uint8_t indexToLight = beatsin8(35, 0, 9);
    settingsStrip[indexToLight] = CHSV(totem->getHue(), 255, 255);
//    Serial.print("Fade: ");
//    Serial.println(totem->getFade());
}

void patternBrightnessMode() {
    if (currentEncoderValue > previousEncoderValue) {
        totem->setBrightness(totem->getBrightness() + 4);
        if(totem->getBrightness() >= 255) {
            flashSettingsRight(CRGB::Red);
        }
    } else if (currentEncoderValue < previousEncoderValue) {
        totem->setBrightness(totem->getBrightness() - 4);
        if(totem->getBrightness() <= 0) {
            flashSettingsLeft(CRGB::Red);
        }
    }

    if (currentEncoderValue != previousEncoderValue) {
        totem->clearStrip();
        (patterns->*pattern_list[currentPattern].pattern)();
    }
    fill_solid( &settingsStrip[0] , 10, CHSV(totem->getHue(), 255, beatsin8(20, 0, 255)));
//    Serial.print("Brightness: ");
//    Serial.println(totem->getBrightness());
}

void patternSaturationMode() {
    if (currentEncoderValue > previousEncoderValue) {
        totem->setSaturation(totem->getSaturation() + 1);
        if(totem->getSaturation() >= 255) {
            flashSettingsRight(CRGB::Red);
        }
    } else if (currentEncoderValue < previousEncoderValue) {
        totem->setSaturation(totem->getSaturation() - 1);
        if(totem->getSaturation() <= 120) {
            flashSettingsLeft(CRGB::Red);
        }
    }
    if (currentEncoderValue != previousEncoderValue) {
        totem->clearStrip();
        (patterns->*pattern_list[currentPattern].pattern)();
    }
    uint8_t indexToLight = beatsin8(40, 0, 9);
    uint8_t saturation = beatsin8(60, 100, 255);
    settingsStrip[indexToLight] = CHSV(totem->getHue(), saturation, 255);

//    Serial.print("Saturation: ");
//    Serial.println(totem->getSaturation());
}

void sensitivitySelectMode() {
    if (currentEncoderValue > previousEncoderValue) {
        equalizer->setSensitivity(Utils::clamp(equalizer->getSensitivity() - 20, 1024));
        if(equalizer->getSensitivity() <= 0) {
            flashSettingsRight(CRGB::Red);
        }
    } else if (currentEncoderValue < previousEncoderValue) {
        equalizer->setSensitivity(Utils::clamp(equalizer->getSensitivity() + 20, 1024));
        if(equalizer->getSensitivity() >= 1024) {
            flashSettingsLeft(CRGB::Red);
        }
    }

    if (currentEncoderValue != previousEncoderValue) {
        totem->clearStrip();
        (patterns->*pattern_list[currentPattern].pattern)();
    }

    uint8_t indexToLight = random8(0, 10);
    EVERY_N_MILLISECONDS(50) {
        clearSettingsStrip();
        for(int i = 0;i<=indexToLight;i++) {
            if(i <= 2) {
                settingsStrip[i] = CRGB::Green;
            } else if(i <= 5) {
                settingsStrip[i] = CRGB::Yellow;
            } else {
                settingsStrip[i] = CRGB::Red;
            }
        }
    }

//    Serial.print("Sensitivity: ");
//    Serial.println(equalizer->getSensitivity());
}

void frequencySelectMode() {
    if (currentEncoderValue > previousEncoderValue) {
        equalizer->setFrequencyOffset(equalizer->getFrequencyOffset() + 1);
    } else if (currentEncoderValue < previousEncoderValue) {
        equalizer->setFrequencyOffset(equalizer->getFrequencyOffset() - 1);
    }

    uint8_t indexToLight = equalizer->getFrequencyOffset() + 1;
    EVERY_N_MILLISECONDS(50) {
        clearSettingsStrip();
        for(int i = 1;i<=indexToLight;i++) {
            if(i <= 2) {
                settingsStrip[i] = CRGB::Green;
            } else if(i <= 5) {
                settingsStrip[i] = CRGB::Yellow;
            } else {
                settingsStrip[i] = CRGB::Red;
            }
        }
    }
}


void nextPattern() {
    currentPattern = Utils::wrap(currentPattern + 1, ARRAY_SIZE(pattern_list) - 1);
    totem->clearStrip();
    (patterns->*pattern_list[currentPattern].pattern)();
}

void previousPattern() {
    currentPattern = Utils::wrap(currentPattern - 1, ARRAY_SIZE(pattern_list) - 1);
    totem->clearStrip();
    (patterns->*pattern_list[currentPattern].pattern)();
}
