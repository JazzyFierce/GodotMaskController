#include <Arduino.h>

// Power source for all LEDs
// Note: May need 3 pins for three lines of the mask?
int const lineLEDs = 3; // PWM Output Pin for varying brightness

// Toggle buttons per mode; digital pins
int const regular = 8;
int const flicker = 7;
int const strobe = 4;
int const turnOff = 2;

// potentiometers
int const brightnessPot = A0;
int const strobePot = A1;

// potentiometer readings
int brightnessReading = 0;
int strobeReading = 0;

// default mode is bright
int current_mode = 0;

// lights up LEDs based on provided brightness, no nonsense
void regularMode(int bright) {
  analogWrite(lineLEDs, bright);
}

// strobes at steady rate based on potentiometer reading
void strobeMode(int bright) {
  // value from 0 - 1023
  strobeReading = analogRead(strobePot);
  // higher strobe reading maps to more flashes per unit time
  // min: 0.5 cycle/sec; max: ~10 cycles per sec
  int strobeRate = (-0.9*strobeReading) + 1000;

  // turn on at specified brightness for specified time
  analogWrite(lineLEDs, bright);
  delay(strobeRate);
  // turn off LEDs for specified time
  analogWrite(lineLEDs, 0);
  delay(strobeRate);
  
}

void flickerMode(int bright) {
  // random delay
  int flicker_delay1 = random(100, 500);
  int flicker_delay2 = random(100, 500);
  int final_delay = random(100, 1500);

  // reduce brightness by 10-50%
  // i.e. have light be 50% - 90% AS BRIGHT as inputted bright variable
  int brightness_redux_1 = random(30, 90);
  int brightness_redux_2 = random(50, 70);
  
  //set LED light levels
  analogWrite(lineLEDs, bright);
  delay(flicker_delay1);
  analogWrite(lineLEDs, 255 - brightness_redux_1);
  delay(flicker_delay2);
  analogWrite(lineLEDs, 255 - brightness_redux_2);
  delay(final_delay);
}

// just. stop.
void finallyPeace() {
  digitalWrite(lineLEDs, LOW);
}

// setup code runs once
void setup() {
  // serial monitor for debugging
  Serial.begin(9600);

  // LEDs
  pinMode(lineLEDs, OUTPUT);

  // set pins connected to toggle buttons
  pinMode(regular, INPUT);
  pinMode(flicker, INPUT);
  pinMode(strobe, INPUT);
  pinMode(turnOff, INPUT);
}



void loop() {
  
  // current_mode starts as 0
  // check all buttons and update mode
  // to keep the monitor clean, button press will only be logged if the mode changes
  if (digitalRead(regular) == HIGH and current_mode != 0) {
    // regular mode activate
    Serial.println("Regular Mode Activated");
    current_mode = 0; }
  if (digitalRead(flicker) == HIGH and current_mode != 1) {
    // flicker mode activate
    Serial.println("Flicker Mode Activated");
    current_mode = 1; }
  if (digitalRead(strobe) == HIGH and current_mode != 2) {
    // strobe mode activate
    Serial.println("Strobe Mode Activated");
    current_mode = 2; }
  if (digitalRead(turnOff) == HIGH and current_mode != 3) {
    // turn off
    Serial.println("Deactivated");
    current_mode = 3;
  } // else current_mode = current_mode

 // grab the brightness real quick like 
 brightnessReading = analogRead(brightnessPot)/4;
 // switch case to call different funcs based on determined mode
  switch(current_mode) {
    case 0:
      regularMode(brightnessReading);
      break;
    case 1:
      flickerMode(brightnessReading);
      break;
    case 2:
      strobeMode(brightnessReading);
      break;
    default:
      digitalWrite(lineLEDs, LOW);
      break;
  }

}
