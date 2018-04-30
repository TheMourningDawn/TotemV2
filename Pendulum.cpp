#include "Pendulum.h"

Pendulum::Pendulum(Torus *totem) : totem(totem) {
    pos = 0;  // Starting center position of pupil
    increment = 2 * 3.14159 / totem->length(); // distance between pixels in radians
    momentumH = 0; // horizontal component of pupil rotational inertia
    momentumV = 0; // vertical component of pupil rotational inertia
}


void Pendulum::readAccelerometerData() {
    // apply a little frictional damping to keep things in control and prevent perpetual motion
    momentumH *= friction;
    momentumV *= friction;

    // Calculate the horizontal and vertical effect on the virtual pendulum
    // 'pos' is a pixel address, so we multiply by 'increment' to get radians.
    float TorqueH = cos(pos * increment);  // peaks at top and bottom of the swing
    float TorqueV = sin(pos * increment);   // peaks when the pendulum is horizontal

    // Add the incremental acceleration to the existing momentum
    // This code assumes that the accelerometer is mounted upside-down, level
    // and with the X-axis pointed forward.  So the Y axis reads the horizontal
    // acceleration and the inverse of the Z axis is gravity.
    // For other orientations of the sensor, just change the axis to match.
    momentumH += TorqueH * CircuitPlayground.motionY() / swing;
    momentumV -= TorqueV * CircuitPlayground.motionX() / gravity;

    // Calculate the new position
    pos += momentumH + momentumV;

    // handle the wrap-arounds
    while (round(pos) < 0) pos += totem->length();
    while (round(pos) > totem->length() - 1) pos -= totem->length();
}

float Pendulum::getMomentumHorizontal() {
    return momentumH;
}

float Pendulum::getMomentumVertical() {
    return momentumV;
}

int Pendulum::getPosition() {
    return pos;
}