#include <Arduino.h>

// put function declarations here:
int LEDRedPin = 13;
int LEDGreenPin = 12;
int LEDBluePin1 = 18;
int LEDBluePin2 = 17;
int LEDBluePin3 = 16;
int ButtonPin = 14;
int LEDBlueState = 0;
int LEDBlueCount = 0;

int ButtonValue = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDRedPin, OUTPUT);
  pinMode(LEDGreenPin, OUTPUT);
  pinMode(LEDBluePin1, OUTPUT);
  pinMode(LEDBluePin2, OUTPUT);
  pinMode(LEDBluePin3, OUTPUT);
  pinMode(ButtonPin, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.begin(115200);
  ButtonValue = digitalRead(ButtonPin);
  if (digitalRead(ButtonPin) == 0) {
    digitalWrite(LEDRedPin, HIGH);
    digitalWrite(LEDGreenPin, LOW);
    if (LEDBlueState == 0) {
      LEDBlueState = 1 ;
    }
  } else {
    digitalWrite(LEDRedPin, LOW);
    digitalWrite(LEDGreenPin, HIGH);
    if (LEDBlueState == 1) {
      LEDBlueState = 0 ;
      LEDBlueCount += 1;
    }
  }

  if (LEDBlueCount % 3 == 0){
    digitalWrite(LEDBluePin1, HIGH);
    digitalWrite(LEDBluePin2, LOW);
    digitalWrite(LEDBluePin3, LOW);
  } else if (LEDBlueCount % 3 == 1){
    digitalWrite(LEDBluePin1, LOW);
    digitalWrite(LEDBluePin2, HIGH);
    digitalWrite(LEDBluePin3, LOW);
  } else if (LEDBlueCount % 3 == 2){
    digitalWrite(LEDBluePin1, LOW);
    digitalWrite(LEDBluePin2, LOW);
    digitalWrite(LEDBluePin3, HIGH);
  }
  delay(50);
}