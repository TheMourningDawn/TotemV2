float pos = 6;  // Starting center position of pupil
float increment = 2 * 3.14159 / NUM_PIXELS; // distance between pixels in radians
float MomentumH = 0; // horizontal component of pupil rotational inertia
float MomentumV = 0; // vertical component of pupil rotational inertia

// Tuning constants. (a.k.a. "Fudge Factors)
// These can be tweaked to adjust the liveliness and sensitivity of the eyes.
const float friction = 0.985; // frictional damping constant.  1.0 is no friction.
const float swing = 20;  // arbitrary divisor for gravitational force
const float gravity = 100;  // arbitrary divisor for lateral acceleration
const float gestureThreshold = -0.80; // accelerometer threshold for toggling modes
const float pupilRadius = 2; // half-width of pupil (in pixels)

void pendulum() {
    Serial.println("Tryna do some pendulum stuff here");
    CRGB magHeadingBasedColor = CHSV(totem->getHue(), 255, 255);
    pendulumMode(magHeadingBasedColor, false, false);
}

void pendulumAntiGravity() {
    CRGB magHeadingBasedColor = CHSV(totem->getHue(), 255, 255);
    pendulumMode(magHeadingBasedColor, true, false);
}

void pendulumMirrored() {
    CRGB magHeadingBasedColor = CHSV(totem->getHue(), 255, 255);
    pendulumMode(magHeadingBasedColor, false, true);
}

void pendulumMode(CRGB color, bool antiGravity, bool mirroredEyes) {
    Serial.println("Starting the pendulum");
    // apply a little frictional damping to keep things in control and prevent perpetual motion
    MomentumH *= friction;
    MomentumV *= friction;

    // Calculate the horizontal and vertical effect on the virtual pendulum
    // 'pos' is a pixel address, so we multiply by 'increment' to get radians.
    float TorqueH = cos(pos * increment);  // peaks at top and bottom of the swing
    float TorqueV = sin(pos * increment);   // peaks when the pendulum is horizontal

    Serial.println("Done setting torque stuffs");

    Serial.println(CircuitPlayground.motionZ());
    Serial.println(CircuitPlayground.motionY());
    Serial.println(CircuitPlayground.motionX());

    // Add the incremental acceleration to the existing momentum
    // This code assumes that the accelerometer is mounted upside-down, level
    // and with the X-axis pointed forward.  So the Y axis reads the horizontal
    // acceleration and the inverse of the Z axis is gravity.
    // For other orientations of the sensor, just change the axis to match.
    MomentumH += TorqueH * CircuitPlayground.motionZ() / swing;
    if (antiGravity) {
        MomentumV += TorqueV * -CircuitPlayground.motionY() / gravity;
    }
    else {
        MomentumV -= TorqueV * -CircuitPlayground.motionY() / gravity;
    }

    Serial.println("Ok, used that accel");

    // Calculate the new position
    pos += MomentumH + MomentumV;

    // handle the wrap-arounds
    while (round(pos) < 0) pos += NUM_PIXELS;
    while (round(pos) > NUM_PIXELS - 1) pos -= NUM_PIXELS;

    Serial.println("Bout half way");

    int lightOn[round(pupilRadius * 2 + 1)];
    int lightIndex = 0;
    for (int i = pos; i > pos - pupilRadius; i--) {
        lightOn[lightIndex] = wrapAround(i, NUM_PIXELS);
        lightIndex++;
    }
    for (int i = pos; i < pos + pupilRadius; i++) {
        lightOn[lightIndex] = wrapAround(i, NUM_PIXELS);
        lightIndex++;
    }
    for (int i = 0; i < NUM_PIXELS; i++) {
        int rightIndex = i;
        boolean turnedOn = false;
        for (int j = 0; j < (pupilRadius * 2 + 1); j++) {
            if (lightOn[j] == i) {
                strip[rightIndex] = color;
                turnedOn = true;
            }
        }
        if (turnedOn == false) {
            strip[rightIndex] = CRGB(0, 0, 0);
        }
    }
    Serial.println("Perty done");
}
