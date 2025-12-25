#include <Arduino.h>

#define DARK_THERSHOLD 300

const int LDR_PIN = 35;
const int RELAY_PIN = 13;

int calculateLux(int ldrValue) {
    return map(ldrValue, 0, 4095, 1000, 0);
}

void setup() {
    Serial.begin(115200);
    pinMode(LDR_PIN, INPUT);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
}

void loop() {
    int ldrValue = analogRead(LDR_PIN);
    int lux = calculateLux(ldrValue);
    Serial.print("LDR Value: ");
    Serial.print(ldrValue);
    Serial.print(" | Lux: ");
    Serial.print(lux);
    if (lux < DARK_THERSHOLD){
        digitalWrite(RELAY_PIN, HIGH);
        Serial.println(" | Turn on Relay");
    } else {
        digitalWrite(RELAY_PIN, LOW);
        Serial.println(" | Turn off Relay");
    }
    delay(500);
}