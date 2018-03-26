/*
 * Methods to change the state of the strip
 */
void fill(CRGB strip[], CRGB color) {
  fill_solid(&(strip[0]), ARRAY_SIZE(strip), color);
}

void fill(CRGB strip[], int hue) {
  CHSV color = CHSV(hue, 255, 255);
  fill_solid(&(strip[0]), ARRAY_SIZE(strip), color);
}

void setPixel(CRGB strip[], uint16_t index) {
	strip[index].setHue(hue);
}

void setPixel(CRGB strip[], uint16_t index, CRGB color) {
	strip[index] = color;
}

void clearStrip() {
	for(uint8_t i=0;i<NUM_PIXELS;i++) {
		strip[i] = CRGB::Black;
	}
}

void cycle_clockwise() {
  shift(strip, animationDirection);
}

//Check out the memmove function to maybe do it more quickly
void shift(CRGB strip[], bool changeDirection) {
	CRGB wrapAroundPixel;
	if (changeDirection == true) {
		wrapAroundPixel = strip[NUM_PIXELS - 1];
		for (int i = NUM_PIXELS - 1; i > 0; i--) {
			strip[i] = strip[i - 1];
		}
		strip[0] = wrapAroundPixel;
	} else {
		wrapAroundPixel = strip[0];
		for (int i = 0; i < NUM_PIXELS; i++) {
			strip[i] = strip[i + 1];
		}
		strip[NUM_PIXELS - 1] = wrapAroundPixel;
	}
}

void fillSolid(uint16_t hue) {
  for (uint8_t i = 0; i < NUM_PIXELS; i++) {
    strip[i].setHue(hue);
  }
}

