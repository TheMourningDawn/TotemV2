void cycleSettingsMode() {
    currentMode = Utils::wrap(currentMode + 1, ARRAY_SIZE(modes) - 1);
    Serial.print("Current settings mode: ");
    Serial.println(currentMode);
    displaySettingMode();
    currentEncoderValue = 0;
    previousEncoderValue = currentEncoderValue; //this is dumb
}

void displaySettingMode() {
    switch (currentMode) {
        case 0:
            setSettingStrip(CRGB::Black, 10);
            settings_strip[0].setHue(15);
            FastLED.show();
            break;
        case 1:
            setSettingStrip(CRGB::Black, 10);
            settings_strip[1].setHue(65);
            FastLED.show();
            break;
        case 2:
            setSettingStrip(CRGB::Black, 10);
            settings_strip[2].setHue(115);
            FastLED.show();
            break;
        case 3:
            setSettingStrip(CRGB::Black, 10);
            settings_strip[3].setHue(165);
            FastLED.show();
            break;
        case 4:
            setSettingStrip(CRGB::Black, 10);
            settings_strip[4].setHue(205);
            FastLED.show();
            break;
        case 5:
            setSettingStrip(CRGB::Black, 10);
            settings_strip[5].setHue(245);
            FastLED.show();
            break;
        case 6:
            setSettingStrip(CRGB::Black, 10);
            settings_strip[6].setHue(255);
            FastLED.show();
            break;
    }

}

void flashSettingsStrip(CRGB color, uint8_t flashDelay, uint8_t flashRepeat) {
    setSettingStrip(color, 10);
    FastLED.show();
    for (uint8_t i = 0; i < flashRepeat; i++) {
        delay(flashDelay);
        setSettingStrip(CRGB::Black, 5);
        FastLED.show();
        delay(flashDelay);
        setSettingStrip(color, 5);
        FastLED.show();
        Serial.println(flashRepeat);
    }
}

void setSettingStrip(CRGB color, uint8_t numberOfDots) {
    for (int i = 0; i < numberOfDots; i++) {
        settings_strip[i] = color;
    }
}

