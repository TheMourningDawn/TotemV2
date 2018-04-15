#include "Equalizer.h"

Equalizer::Equalizer() {
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

