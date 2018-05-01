void cycleSettingsMode() {
    currentMode = Utils::wrap(currentMode + 1, ARRAY_SIZE(modes) - 1);
    displaySettingMode();
    currentEncoderValue = 0;
    previousEncoderValue = currentEncoderValue; //this is dumb
}

void clearSettingsStrip() {
    for (int i = 0; i < 10; i++) {
        settings_strip[i] = CRGB::Black;
    }
}

void displaySettingMode() {
    clearSettingsStrip();
    switch (currentMode) {
        // Animation mode
        case 0:
            settings_strip[0].setHue(205);
            FastLED.show();
            break;
        // Speed mode
        case 1:
            settings_strip[1].setHue(155);
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
            settings_strip[5].setHue(90);
            FastLED.show();
            break;
        // Sensitivity mode
        case 6:
            settings_strip[6].setHue(0);
            FastLED.show();
            break;
        // Frequency mode
        case 7:
            settings_strip[7].setHue(205);
            FastLED.show();
            break;
        case 8:
            settings_strip[8].setHue(60);
            FastLED.show();
            break;
        case 9:
            settings_strip[9].setHue(90);
            FastLED.show();
            break;
    }

}



