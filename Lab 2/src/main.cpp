#include <Arduino.h>

const int LDR_PIN = 23;
const int RELAY_PIN = 22;

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  int ldrValue = digitalRead(LDR_PIN);
  Serial.println(ldrValue);
  if (ldrValue == HIGH){
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }
  delay(50);
}