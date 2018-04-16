#include "Utils.h"

//Utils::Utils() {}

/********************************************************
 * Bounds a integer between 0 and the maxValue inclusive
 *
 * Any value above maxValue is set to maxValue
 * Any value below 0 is set to 0
 */
int Utils::clamp(int value, int maxValue) {
    return Utils::clamp(value, 0, maxValue);
}


/********************************************************
 * Bounds a integer between minValue and the maxValue
 *
 * Any value above maxValue is set to maxValue
 * Any value below minValue is set to minValue
 */
int Utils::clamp(int value, int minValue, int maxValue) {
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}

/********************************************************
 * Wraps a integer an equal distance in the same direction
 * past the opposite extreme if it passes one extreme
 *
 * Any value such that value = 0 - X is set to maxValue - X
 * Any value such that value = maxValue + X is set to 0 + X
 */
int Utils::wrap(int value, int maxValue) {
    return Utils::wrap(value, 0, maxValue);
}

/********************************************************
 * Wraps a integer an equal distance in the same direction
 * past the opposite extreme if it passes one extreme
 *
 * Any value such that value = minValue - X is set to maxValue - X
 * Any value such that value = maxValue + X is set to minValue + X
 */
int Utils::wrap(int value, int minValue, int maxValue) {
    int range = maxValue - minValue + 1;
    if(value > maxValue) {
        value = minValue + (value % maxValue);
    } else if (value < minValue) {
        value = value + range;
    }
    return value;
}

