#include <Adafruit_NeoPixel.h>
#include "Adafruit_FreeTouch.h"
#include "HID-Project.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_NEOPIXEL, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

Adafruit_FreeTouch qt = Adafruit_FreeTouch(PIN_TOUCH, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);

int16_t neo_brightness = 255;

bool last_switch = true;
bool last_touch = true;
bool is_color_wheel_on = true;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(neo_brightness);
  strip.show();

  if (! qt.begin()) {
    Serial.println("Failed to begin qt");
  }

  pinMode(PIN_SWITCH, INPUT_PULLDOWN);

  //Consumer.begin();
  Keyboard.begin();
}

void onTouched() {
    Serial.println("Touched");
    is_color_wheel_on = !is_color_wheel_on;
}

void onTouchReleased() {
    Serial.println("Touch Released");
}

void onPressed() {
    Serial.println("Pressed");

    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('.');
    delay(10);
    Keyboard.release('.');
    Keyboard.release(KEY_LEFT_GUI);
    delay(100);
    Keyboard.print("facepalm");
    delay(100);
    Keyboard.press(KEY_RIGHT_ARROW);
    delay(100);
    Keyboard.release(KEY_RIGHT_ARROW);
    delay(100);
    Keyboard.press(KEY_ENTER);
    delay(100);
    Keyboard.release(KEY_ENTER);
    delay(100);
    Keyboard.press(KEY_ESC);
    delay(100);
    Keyboard.releaseAll();

}

void onReleased() {
    Serial.println("Released");
}

uint8_t j = 0;
void loop() {
  
  // check touch
  bool curr_touched = qt.measure() > 500;
  if (curr_touched != last_touch) {
    if (curr_touched) {
      onTouched();
    } else {
      onTouchReleased();
    }
    last_touch = curr_touched;
  }
  
  // check mechswitch
  bool curr_switch = digitalRead(PIN_SWITCH);
  if (curr_switch != last_switch) {
    if (curr_switch) {
      onPressed();
    } else {
      onReleased();
    }
    last_switch = curr_switch;
  }

  if (is_color_wheel_on) {
    strip.setBrightness(neo_brightness);
    strip.setPixelColor(0, Wheel(j++));
  } else {
    strip.setBrightness(0);
  }
  
  strip.show();

  delay(10);
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
