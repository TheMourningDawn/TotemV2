void cycleSettingsMode() {
    currentMode = Utils::wrap(currentMode + 1, ARRAY_SIZE(modes) - 1);
    displaySettingMode();
    currentEncoderValue = 0;
    previousEncoderValue = currentEncoderValue; //this is dumb
}

void clearSettingsStrip() {
    for (int i = 0; i < 10; i++) {
        settingsStrip[i] = CRGB::Black;
    }
}

void spinSettingsClockwise() {
    CRGB firstLed = settingsStrip[0];
    memmove(&settingsStrip[0], &settingsStrip[1], (9) * sizeof(CRGB));
    settingsStrip[9] = firstLed;
}

void spinSettingsCounterClockwise() {
    CRGB lastLed = settingsStrip[9];
    memmove(&settingsStrip[1], &settingsStrip[0], (9) * sizeof(CRGB));
    settingsStrip[0] = lastLed;
}

void flashSettingsRight(CRGB color) {
    CRGB previousSettingsStrip[NUM_SETTING_PIXELS];
    memcpy( previousSettingsStrip, settingsStrip, NUM_SETTING_PIXELS * sizeof(CRGB));
    for(int i=5;i<10;i++) {
        settingsStrip[i] = color;
    }
    FastLED.show();
    memcpy(settingsStrip, previousSettingsStrip, NUM_SETTING_PIXELS * sizeof(CRGB));
}

void flashSettingsLeft(CRGB color) {
    CRGB previousSettingsStrip[NUM_SETTING_PIXELS];
    memcpy( previousSettingsStrip, settingsStrip, NUM_SETTING_PIXELS * sizeof(CRGB));
    for(int i=0;i<5;i++) {
        settingsStrip[i] = color;
    }
    FastLED.show();
    memcpy(settingsStrip, previousSettingsStrip, NUM_SETTING_PIXELS * sizeof(CRGB));
}

void displaySettingMode() {
    clearSettingsStrip();
    switch (currentMode) {
        // Animation mode
        case 0:
            for(int i = 0;i < 10; i++) {
                settingsStrip[i] = CHSV(i*255/10, totem->getSaturation(), totem->getBrightness());
            }
            break;
        // Speed mode
        case 1:
            settingsStrip[1].setHue(155);
            FastLED.show();
            break;
        // Color mode
        case 2:
            break;
        // Brightness mode
        case 3:
            break;
        // Saturation mode
        case 4:
            break;
        // Fade mode
        case 5:
//            settingsStrip[5].setHue(90);
//            FastLED.show();
            break;
        // Sensitivity mode
        case 6:
//            settingsStrip[6].setHue(0);
//            FastLED.show();
            break;
        // Frequency mode
        case 7:
//            settingsStrip[7].setHue(205);
//            FastLED.show();
            break;
        case 8:
//            settingsStrip[8].setHue(60);
//            FastLED.show();
            break;
        case 9:
//            settingsStrip[9].setHue(90);
//            FastLED.show();
            break;
    }

}



