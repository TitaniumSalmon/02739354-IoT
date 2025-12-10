#include <Arduino.h>
#include <liquidcrystal_i2c.h>
#include <DHT.h>

//  MARK: LCD setup  
LiquidCrystal_I2C lcd(0x27, 16, 2);
int count = 0;
void initLCD() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting...!");
  delay(2000);
}
void displayLCD(int col, int row, String msg, bool clear = true){
  if(clear) {
    lcd.clear();
  }
  lcd.setCursor(col, row);
  lcd.print(msg);
}

//  MARK: DHT setup
#define DHTPIN 16     // ขา Data(SDA) เสียบช่องไหน
#define DHTTYPE DHT22 // รุ่น DHT
DHT dht(DHTPIN, DHTTYPE);

int LEDRedPin = 33;
int LEDYellowPin = 25;
int LEDBluePin = 32;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initLCD();
  dht.begin();

  pinMode(LEDRedPin, OUTPUT);
  pinMode(LEDYellowPin, OUTPUT);
  pinMode(LEDBluePin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)){
    displayLCD(0,0, "Failed to read!");
    return;
  }

  // TOO HOT
  if (temperature > 35){
    digitalWrite(LEDRedPin, HIGH);
  } else {
    digitalWrite(LEDRedPin, LOW);
  }

  // TOO COLD
  if (temperature < 15){
    digitalWrite(LEDBluePin, HIGH);
  } else {
    digitalWrite(LEDBluePin, LOW);
  }

  // HUMIDITY SQUAD
  if (humidity < 30){
    digitalWrite(LEDYellowPin, HIGH);
  } else {
    digitalWrite(LEDYellowPin, LOW);
  }

  displayLCD(0, 0, "Hum " + String(humidity) + " %",true);
  displayLCD(0, 1, "Temp " + String(temperature) + " C",false);
}