#include "mbed.h"

// efficient integer powers x**p
int intPow(int x, unsigned int p) {
  if (p == 0)
    return 1;
  if (p == 1)
    return x;
  int tmp = intPow(x, p / 2);
  if (p % 2 == 0)
    return tmp * tmp;
  else
    return x * tmp * tmp;
}

// Activity 1
DigitalOut leds[3] = {LED1, LED2, LED3}; // {green, blue, red}
InterruptIn button(BUTTON1);
Ticker tickLED;
Timeout buttonDebounce;
size_t ledCycle = 0;   // index of leds[]
unsigned int ledQ = 0; // base-3 encoded queue
size_t qLength = 0;
size_t currLED = 0;
bool pressedThisCycle = false;
bool recording = false;

void onButtonStopDebouncing();
void onButtonPress() {
  recording = !pressedThisCycle;
  pressedThisCycle = true;
  ledQ += currLED * intPow(3, qLength); // push new index to front of base-3 encoding
  qLength++;
  button.rise(NULL);
  buttonDebounce.attach(onButtonStopDebouncing, 50ms);
}
void onButtonStopDebouncing() { button.rise(onButtonPress); }

void update() {
  pressedThisCycle = false;
  leds[0] = false;
  leds[1] = false;
  leds[2] = false;
  if (recording || !ledQ) {
    currLED = ledCycle;
    ledCycle = (ledCycle + 1) % 3;
  } else {
    currLED = ledQ % 3; // pop oldest index from back of base-3 encoding
    ledQ /= 3;
    qLength--;
  }
  leds[currLED] = true;
}

int main() {
  tickLED.attach(update, 750ms);
  button.rise(onButtonPress);
  while (true) {
  }
  return 0;
}