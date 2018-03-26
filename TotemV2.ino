#include <Wire.h>
#include <FastLED.h>
#include "TimerOne.h"
#include "ClickEncoder.h"
#include <Adafruit_CircuitPlayground.h>

#define STRIP_PIN 12
#define SETTINGS_STRIP_PIN 17

#define PIN_ENCODER_A 0
#define PIN_ENCODER_B 1
#define PIN_ENCODER_SWITCH 6

#define NUM_PIXELS 80
#define NUM_SETTING_PIXELS 10

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

ClickEncoder *encoder;
int16_t previousEncoderValue, currentEncoderValue;

CRGB strip[NUM_PIXELS];
CRGB settings_strip[NUM_SETTING_PIXELS];

bool flipFlopState = true;

uint8_t hue = 0;
uint8_t currentMode = 0;
uint8_t currentPattern = 0;
uint16_t animationSpeed = 24;
bool animationDirection = true;

typedef void (*SimplePattern)();
typedef void (*Action)();
typedef void (*Mode)();

typedef struct {
    SimplePattern pattern;
    Action action;
} PatternDefinition;
typedef PatternDefinition PatternDefinitionList[];

// It's silly this is here...but for some reason stuff doesn't work if it's not...?
void nothing() {
  //notin
}

const PatternDefinitionList patterns = {
        {meteor, cycle_clockwise},
        {fourPoints, cycle_clockwise},
        {nothing,       bpm},
        {nothing,       juggle},
        {nothing,       sinelon},
        {nothing,       confetti},
        {nothing,       chasingFromSides},
        {nothing,       chasingInfinity},
        {nothing,       middleFanout},
        {wipeRainbow,   cycle_clockwise},
        {nothing,       pendulum}
};

typedef Mode ModeList[];
const ModeList modes = {
    patternSelectMode,
    patternSpeedMode,
    patternColorMode
};

void timerIsr() {
  encoder->service();
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");

  // Initialize the circuit playground board
  CircuitPlayground.begin();

  //Initialize FastLED for the main strip and the settings strip
  FastLED.addLeds<NEOPIXEL, STRIP_PIN>(strip, NUM_PIXELS);
  FastLED.addLeds<NEOPIXEL, SETTINGS_STRIP_PIN>(settings_strip, NUM_SETTING_PIXELS);

  //Initialize the encoder (knob) and it's interrupt
  encoder = new ClickEncoder(0, 1, 6, 4, false);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  //Initial value for the previous encoder value
  previousEncoderValue = 0;

  // Display the initial pattern
  displaySettingMode();
  patterns[currentPattern].pattern();
}

void loop() {
  checkEncoderInput();
  patterns[currentPattern].action();
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
        cycleSettingsMode();
        break;
      case ClickEncoder::DoubleClicked:
        encoder->setAccelerationEnabled(!encoder->getAccelerationEnabled());
        break;
      case ClickEncoder::Released:
        flipFlop(animationDirection);
        break;
      default:
        ;
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
  if (animationSpeed <= 4) {
    animationSpeed = 5;
  }
}

void patternColorMode() {
  hue += 5;
  clearStrip();
  patterns[currentPattern].pattern();
}

void nextPattern() {
    Serial.println("Doing the next one, boss");
    currentPattern = wrapAround(currentPattern + 1, ARRAY_SIZE(patterns));
    clearStrip();
    patterns[currentPattern].pattern();
}

void previousPattern() {
    currentPattern = wrapAround(currentPattern - 1, ARRAY_SIZE(patterns));
    clearStrip();
    patterns[currentPattern].pattern();
}

int wrapAround(int value, int maxValue) {
    if (value < 0) {
        return value + maxValue;
    }
    if (value > maxValue - 1) {
        return value - maxValue;
    }
    return value;
}

void flipFlop(bool &flopToFlip) {
  if (flopToFlip == true) {
    flopToFlip = false;
    return;
  }
  flopToFlip = true;
}
