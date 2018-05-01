#ifndef PENDULUM_H
#define PENDULUM_H

#include "Torus.h"
#include <Adafruit_CircuitPlayground.h>

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

class Pendulum {
private:
    Torus *totem;
    //All pendulum related fields. Really I think we should just move this into another classsssssss
    float pos;  // Starting center position of pupil
    float increment; // distance between pixels in radians
    float momentumH; // horizontal component of pupil rotational inertia
    float momentumV; // vertical component of pupil rotational inertia

    // Tuning constants. (a.k.a. "Fudge Factors)
    // These can be tweaked to adjust the liveliness and sensitivity of the pendulum.
    const float friction = 0.996; // frictional damping constant.  1.0 is no friction.
    const float swing = 20;  // arbitrary divisor for gravitational force
    const float gravity = 150;  // arbitrary divisor for lateral acceleration
public:
    Pendulum(Torus *totem);

    void readAccelerometerData();
    float getMomentumHorizontal();
    float getMomentumVertical();
    int getPosition();
};

#endif //PENDULUM_H
