/*
 * These patterns do all of their operations within the function
 * this is bad because it'll be delaying the main loop until it ends
 * settings will not chanage right away, color sensor wont update, etc
 * though, most of these last 1-2 seconds.
 */

void halfTopBottom() {
  halfTopBottom(true, CRGB::Red, CRGB::Blue);
}

void halfTopBottom(bool animate, CRGB colorTop, CRGB colorBottom) {
	Serial.println("Starting function");
	uint16_t rightAnchor = 15;
	uint16_t leftAnchor = 45;
	uint16_t endPoint = 45;

	while (rightAnchor < endPoint) {
		setPixel(strip, rightAnchor, colorTop);
		setPixel(strip, leftAnchor, colorBottom);
		rightAnchor++;
		leftAnchor++;
		if (leftAnchor == NUM_PIXELS) {
			leftAnchor = 0;
		}
		if (animate == true) {
			FastLED.show();
			delay(animationSpeed);
		}
	}
	if (animate == false) {
		FastLED.show();
	}
}

void middleFanout() {
	uint8_t left, right;
	if (flipFlopState == true) {
		left = 0;
		right = NUM_PIXELS - 1;
		while (left < 30 && right > 30) {
			setPixel(strip, left);
			setPixel(strip, right);
			FastLED.show();
			left += 1;
			right -= 1;
			//If it's the last pixel, dont wait to start the next thing.
			if (right != 0) {
				delay(animationSpeed);
			}
		}
		flipFlop(flipFlopState);
	} else {
		left = 30;
		right = 30;
		while (right > 0 && left < 60) {
			setPixel(strip, left);
			setPixel(strip, right);
			FastLED.show();

			left += 1;
			right -= 1;

			//If it's the last pixel, dont wait to start the next thing.
			if (right != 0) {
				delay(animationSpeed);
			}
		}
		flipFlop(flipFlopState);
	}
	hue += 32;
}

void chasingInfinity() {
  chasingInfinity(true);
}

void chasingInfinity(bool changeChaseDirection) {
	uint8_t i;
	if (changeChaseDirection == true) {
		for (i = 0; i < NUM_PIXELS; i++) {
			setPixel(strip, i);
			FastLED.show();
			delay(animationSpeed);
		}
	} else {
		for (i = NUM_PIXELS; i >= 1; --i) {
			setPixel(strip, i - 1);
			FastLED.show();
			delay(animationSpeed);
		}
	}
	hue += 33;
}

void chasingFromSides() {
  chasingFromSides(true);
}

void chasingFromSides(bool changeChaseDirection) {
	uint8_t leftTop, rightTop, leftBottom, rightBottom;
	leftTop = 45; //45 -> 30
	leftBottom = 45; //45 -> 60
	rightTop = 15; //15 -> 0
	rightBottom = 15; //15 -> 30
	while (leftBottom < 60 && leftTop >= 30 && rightBottom < 30 && rightTop >= 0) {
		setPixel(strip, leftTop);
		setPixel(strip, leftBottom);
		setPixel(strip, rightTop);
		setPixel(strip, rightBottom);
		FastLED.show();
		leftTop--;
		leftBottom++;
		rightTop--;
		rightBottom++;
		delay(animationSpeed);
	}
	setPixel(strip, leftTop);
	setPixel(strip, rightTop);

	hue += 33;
}
