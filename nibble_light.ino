#include <FastLED.h>
#include <EEPROM.h>

#define NUM_LEDS 10

// pins
#define ENC_BUTTON 6
#define LED 7
#define ENC_A 8
#define ENC_B 9

#define ENCODER_HUE_CHANGE 8
#define ENCODER_VALUE_CHANGE 8
#define SERIAL_HUE_CHANGE 8
#define SERIAL_VALUE_CHANGE 16

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

  Serial.begin(9600);

  // persistent data
  hue     = EEPROM.read(HUE);
  value   = EEPROM.read(VALUE);
  enabled = EEPROM.read(ENABLED);

  // turn off unnecessary lights
  pinMode(LED_BUILTIN_TX,INPUT);
  pinMode(LED_BUILTIN_RX,INPUT);

  updateLEDs();
}

void loop() {
  // button
  int button = digitalRead(ENC_BUTTON);
  // only run if it is newly pressed
  if(button == 0 && button_last != 0)
    press();

  // encoder
  int enc_a = digitalRead(ENC_A);
  // only run if value is new
  if(enc_a == 0 && a_last != 0) { // 
    int enc_b = digitalRead(ENC_B);

    if(enc_b == enc_a)
      rotate(1);
    else
      rotate(-1);
  }

  // serial
  if(Serial.available()) {
    String str = Serial.readStringUntil('\n');\
    str.trim();
    handleStr(str);
  }

  a_last = enc_a;
  button_last = button;

  delay(1); // slight delay required for debouncing
}

// value changes
void changeHue(int amt) {
  hue += amt; updateHue();
  updateLEDs();
}

void changeValue(int amt) {
  value = max(0, min(255, value + amt)); updateValue();
  updateLEDs();
}

void toggleEnabled() {
  enabled = !enabled; updateEnabled();
  updateLEDs();
}

// led stuff
void updateLEDs() {
  setLEDs(hue, enabled ? value : 0);
}

void setLEDs(int hue, int value) {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, 255, value);
  }

  FastLED.show(); 
}

// serial
void handleStr(String str) {
  // get command
  if(str.length() < 1) return;
  char command = str.charAt(0);

  // single argument commands
  if(command == 'e') { toggleEnabled(); return; }

  // get direction
  if(str.length() < 2) return;
  char val = str.charAt(1);
  int dir = getDirection(val);

  // direction commands
  switch(command) {
    case 'h':
      changeHue(dir * SERIAL_HUE_CHANGE);
      break;
    case 'v':
      changeValue(dir * SERIAL_VALUE_CHANGE);
      break;
  }
}

int getDirection(char chr) {
  switch(chr) {
    case 'r':
    case '+':
      return 1;
    case 'l':
    case '-':
      return -1;
    default:
      return 0;
  }
}

// encoder
void rotate(int dir) {
  switch (state) {
    case HUE:
      changeHue(dir * ENCODER_HUE_CHANGE);
      break;
    case VALUE:
      changeValue(dir * ENCODER_VALUE_CHANGE);
      break;
    case ENABLED:
      toggleEnabled();
      break;
  }
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
