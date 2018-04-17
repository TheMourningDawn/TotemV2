#include "Equalizer.h"

Equalizer::Equalizer() {
    sensitivity = 100;
    frequencyOffset = 0;

    pinMode(RESET_PIN, OUTPUT); // reset
    pinMode(STROBE_PIN, OUTPUT); // strobe
    digitalWrite(RESET_PIN, LOW); // reset low
    digitalWrite(STROBE_PIN, HIGH); //pin 5 is RESET on the shield
}

void Equalizer::readAudioFrequencies() {
    // reset the data
    digitalWrite(RESET_PIN, HIGH);
    digitalWrite(RESET_PIN, LOW);

    // loop through all 7 bands
    for (int band = 0; band < 7; band++) {
        digitalWrite(STROBE_PIN, LOW); // go to the next band
        delayMicroseconds(50); // gather some data
        frequencies[band] = analogRead(EQ_PIN); // store left band reading
        digitalWrite(STROBE_PIN, HIGH); // reset the strobe pin
    }
//    for(int i=0;i<7;i++) {
//        Serial.print(frequencies[i]);
//        Serial.print(" ");
//    }
//    Serial.println("");
}

uint8_t Equalizer::getFrequencyOffset() {
    return frequencyOffset;
}

void Equalizer::setFrequencyOffset(uint8_t newFrequencyOffset) {
    frequencyOffset = newFrequencyOffset;
}

void Equalizer::resetFrequencyOffset() {
    frequencyOffset = 0;
}

uint16_t Equalizer::getSensitivity() {
    return sensitivity;
}

void Equalizer::setSensitivity(uint16_t newSensitivity) {
    sensitivity = newSensitivity;
}

uint8_t Equalizer::getBand(uint8_t band) {
    return map(frequencies[band], sensitivity, 1025, 0, 255);
}

uint8_t Equalizer::getBand(uint8_t band, uint16_t sensitivityThreshold) {
    return map(frequencies[band], sensitivityThreshold, 1025, 0, 255);
}

