#ifndef PATTERNS_H
#define PATTERNS_H

#include "TimerOne.h"
#include "Torus.h"

FASTLED_USING_NAMESPACE;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

class Patterns {
private:
    Torus *totem;
public:
    Patterns();
    Patterns(Torus *totem);
    void nothing();
    void rainbow();
    void fourPoints();
    void halfTopBottom();
    void halfTopBottom(bool animate, CRGB colorTop, CRGB colorBottom);
    void fourSquare();

};

#endif
