#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>

// WIFI config
const char *ssid = "Wokwi-GUEST"; // setup SSID of WiFi
const char *password = "";        // setup WiFi password
WiFiClient wifiClient;
// MQTT client config
const char *mqttServer = "broker.hivemq.com"; // setup MQTT broker URL or IP Address
const int mqttPort = 1883;                    // setup MQTT port
const char *mqttUser = "";                    // setup MQTT user
const char *mqttPassword = "";                // setup MQTT password
const char *topic_pub = "";                   // setup topic name for publishing
const char *topic_sub = "Inwza007";           // setup topic name for subscribing
PubSubClient mqttClient(wifiClient);
// some variables buffer
String publishMessage;

int count = 0;
int LEDRedPin = 33;
int LEDYellowPin = 25;
int LEDBluePin = 32;

LiquidCrystal_I2C lcd(0x27, 16, 2);
void initLCD()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting...!");
  delay(2000);
  lcd.clear();
}
void displayLCD(int col, int row, String msg, bool clear = true)
{
  if (clear)
  {
    lcd.clear();
  }
  lcd.setCursor(col, row);
  lcd.print(msg);
}

//
// ** variable for testing MQTT publish
//
// int counter = 0;
// const int BUTTON_PIN = 22; // GPIO22
//

void setup_wifi()
{
  // Start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi connected on ");
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.print(" MAC address: ");
  Serial.println(WiFi.macAddress());
}

void reconnectMQTT()
{
  // Loop until reconnected
  char mqttinfo[80];
  char clientId[100] = "\0";
  snprintf(mqttinfo, 75, "Attempting MQTT connection at %s:%d (%s/%s)...",
           mqttServer, mqttPort, mqttUser, mqttPassword);
  while (!mqttClient.connected())
  {
    Serial.println(mqttinfo);
    if (mqttClient.connect(clientId))
    {
      Serial.println("---> MQTT Broker connected...");
      // subscribe here after connected
      mqttClient.subscribe(topic_sub);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000); // Recommended: Wait 5 seconds before retrying
    }
  }
}

void mqttCallback(char *topic, byte *payload, unsigned int lenght)
{
  char Temp[128];
  char Humidity[128];
  bool behindspace = false;
  char payloadMsg[128];
  int tempIdx = 0, humIdx = 0;
  Serial.print("---> Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("     Message:");
  for (int i = 0; i < lenght; i++)
  {
    Serial.print((char)payload[i]);
    payloadMsg[i] = (char)payload[i];
    if(payloadMsg[i] == ' '){
      behindspace = true;
    } else if(!behindspace){
      Temp[tempIdx++] = payload[i];
    } else {
      Humidity[humIdx++] = payload[i];
    }
  }
payloadMsg[lenght] = '\0';
Temp[tempIdx] = '\0';
Humidity[humIdx] = '\0';
Serial.println();
Serial.println(Temp);
Serial.println(Humidity);
Serial.println("-----------------------");

int tempValue = atoi(Temp);
int humidityValue = atoi(Humidity);

if (tempValue > 35){
  digitalWrite(LEDRedPin, HIGH);
} else {
  digitalWrite(LEDRedPin, LOW);
}

// TOO COLD
if (tempValue < 15){
  digitalWrite(LEDBluePin, HIGH);
} else {
  digitalWrite(LEDBluePin, LOW);
}

// HUMIDITY SQUAD
if (humidityValue < 30){
  digitalWrite(LEDYellowPin, HIGH);
} else {
  digitalWrite(LEDYellowPin, LOW);
}

  displayLCD(0, 0, "Hum " + String(Humidity) + " %",true);
  displayLCD(0, 1, "Temp " + String(Temp) + " C",false);
}

void setup()
{
  Serial.begin(115200);
  initLCD();
  setup_wifi();
  pinMode(LEDRedPin, OUTPUT);
  pinMode(LEDYellowPin, OUTPUT);
  pinMode(LEDBluePin, OUTPUT);
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(mqttCallback);
}

void loop()
{
  if (!mqttClient.connected())
  {
    reconnectMQTT();
  }
  mqttClient.loop();
  delay(500); // this speeds up the simulation
}