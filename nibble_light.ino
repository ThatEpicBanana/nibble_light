#include <FastLED.h>
#include <EEPROM.h>

#define NUM_LEDS 10

// pins
#define ENC_BUTTON 6
#define LED 7
#define ENC_A 8
#define ENC_B 9

#define HUE_CHANGE 8
#define VALUE_CHANGE 8

// eeprom locations / state
#define HUE 0
#define VALUE 1
#define ENABLED 2

#define updateHue()     EEPROM.update(HUE, hue);
#define updateValue()   EEPROM.update(VALUE, value);
#define updateEnabled() EEPROM.update(ENABLED, enabled);

CRGB leds[NUM_LEDS];

int hue = 0;
int value = 255;
bool enabled = true;

int state = 0;

int button_last = 1;
int a_last = 0;

void setup() {
  FastLED.addLeds<WS2812B, LED, GRB>(leds, NUM_LEDS);

  // remember 1 is off, 0 is on
  pinMode(ENC_BUTTON, INPUT_PULLUP);
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);

  // Serial.begin(9600);

  // persistent data
  hue     = EEPROM.read(HUE);
  value   = EEPROM.read(VALUE);
  enabled = EEPROM.read(ENABLED);

  updateLEDs();
}

void loop() {
  int enc_a = digitalRead(ENC_A);
  int button = digitalRead(ENC_BUTTON);

  if(button == 0 && button_last != 0)
    press();

  // only keep values if they're new
  if(enc_a == 0 && a_last != 0) { // 
    int enc_b = digitalRead(ENC_B);

    if(enc_b == enc_a)
      rotate(1);
    else
      rotate(-1);
  }

  a_last = enc_a;
  button_last = button;

  delay(1); // slight delay required for debouncing
}

void rotate(int dir) {
  switch (state) {
    case HUE:
      hue += dir * HUE_CHANGE; updateHue();
      break;
    case VALUE:
      value = max(0, min(255, value + dir * VALUE_CHANGE)); updateValue();
      break;
    case ENABLED:
      enabled = !enabled; updateEnabled();
      break;
  }

  updateLEDs();
}

void press() {
  // update state
  state += 1;

  if(state > ENABLED) // out of bounds
    state = 0;

  // small animation
  switch (state) {
    case HUE:
      for(int hueOffset = 0; hueOffset < 256; hueOffset++) {
        setLEDs(hue + hueOffset, value);
        delay(1);
      } break;
    case VALUE:
      for(int newvalue = 0; newvalue < 256; newvalue++) {
        setLEDs(hue, newvalue);
        delay(1);
      } break;
    case ENABLED:
      setLEDs(hue, 0);
      delay(256);
      break;
  }

  // go back to original
  updateLEDs();
}

void updateLEDs() {
  setLEDs(hue, enabled ? value : 0);
}

void setLEDs(int hue, int value) {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, 255, value);
  }

  FastLED.show(); 
}