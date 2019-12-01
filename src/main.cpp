#define EXT_INT 2
#define F3 3
#define F2R 5
#define F2L 6
#define F1L 9
#define F1R 10
#define F1M 11

#include <Arduino.h>

int leds[6] = { F1L, F1R, F1M, F2L, F2R, F3 };

uint8_t selected_pin = 0;
unsigned int selection_timeout = 250;
unsigned int buttonTimer = 0;
bool buttonActive = false;
bool toggle = true;

void select_pin();
void toggle_led();

void setup() {
  // put your setup code here, to run once:
  for (uint8_t i=0; i <= 5; i++) {
    pinMode(leds[i], OUTPUT);
  }

  pinMode(EXT_INT, INPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(EXT_INT) == HIGH) {

		if (buttonActive == false) {
			buttonActive = true;
			buttonTimer = millis();
		}

    // check long press
    if (millis() - buttonTimer >= selection_timeout) {
      // toggle led when button released
      if (toggle) {
        Serial.println("long");
        toggle_led();
      }
    } else {
      if (digitalRead(EXT_INT) == LOW) {
        Serial.println("short");
        select_pin();
        
      }
    }
  } else {
    buttonActive = false;
    toggle = true;
  }
}

void select_pin() {
  if (selected_pin >= 5) {
    selected_pin = 0;
  } else {
    selected_pin += 1;
  }

  Serial.println("Current LED: "); Serial.print(leds[selected_pin]);
  int led_timer_start = millis();
  if (millis() - led_timer_start < selection_timeout) {
    analogWrite(leds[selected_pin], 50);
  }
  
}

void toggle_led() {
  bool state = digitalRead(leds[selected_pin]);
  digitalWrite(leds[selected_pin], !state);
  toggle = false;
}
