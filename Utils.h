#ifndef UTILS_H
#define UTILS_H

class Utils {
public:
    static int clamp(int value, int maxValue);
    static int clamp(int value, int minValue, int maxValue);
    static int wrap(int value, int maxValue);
    static int wrap(int value, int minValue, int maxValue);
};

#endif //UTILS_H
