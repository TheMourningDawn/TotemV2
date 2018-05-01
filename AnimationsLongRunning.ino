//*
//
//void middleFanout() {
//	uint8_t left, right;
//	if (flipFlopState == true) {
//		left = 0;
//		right = NUM_PIXELS - 1;
//		while (left < 30 && right > 30) {
//			setPixel(strip, left);
//			setPixel(strip, right);
//			FastLED.show();
//			left += 1;
//			right -= 1;
//			//If it's the last pixel, dont wait to start the next thing.
//			if (right != 0) {
//				delay(animationSpeed);
//			}
//		}
//		flipFlop(flipFlopState);
//	} else {
//		left = 30;
//		right = 30;
//		while (right > 0 && left < 60) {
//			setPixel(strip, left);
//			setPixel(strip, right);
//			FastLED.show();
//
//			left += 1;
//			right -= 1;
//
//			//If it's the last pixel, dont wait to start the next thing.
//			if (right != 0) {
//				delay(animationSpeed);
//			}
//		}
//		flipFlop(flipFlopState);
//	}
//	hue += 32;
//}
