#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
int outputState[4] = {0, 0, 0, 0};
// WIFI config
const char *ssid = "Wokwi-GUEST";
const char *password = "";
WiFiClient wifiClient;
// MQTT client config
const char *mqttServer = "mqtt.netpie.io";
const int mqttPort = 1883;
const char *mqttClientId = "8af8b2f4-56a9-49a0-965c-00e9ec178220";
const char *mqttUser = "Q6J6VXKhpUeK9xjBXGqbLCEuwJiGYcjF";
const char *mqttPassword = "GoRcwKXPwmrKv8R7WSsyTi2zvmEoHsmM";
const char *topic_sub = "@msg/home/device_control"; // topic to subscribe
const char *topic_pub = "@msg/home/device_status"; // topic to publish
const char *data_pub = "@shadow/data/update"; // topic to publish
PubSubClient mqttClient(wifiClient);

// pin definition
const int RELAY_LED_RED_PIN = 12; 
const int RELAY_LED_YELLOW_PIN = 14;
const int RELAY_LED_GREEN_PIN = 27;
const int BUZZER_PIN = 19;

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
    
    if (mqttClient.connect(mqttClientId, mqttUser, mqttPassword))
    {
      Serial.println("---> MQTT Broker connected...");
      // subscribe here after connected
      mqttClient.subscribe(topic_sub);
      Serial.println("Subscribed to topic: " + String(topic_sub));
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");      
      delay(5000);  // Recommended: Wait 5 seconds before retrying
    }
  }
}

void publishLightStatus(const char* status_1, const char* status_2, const char* status_3, const char* status_4) {
  String publishMessage;
  // Prepare JSON message
  publishMessage = "{\"data\":{";
  publishMessage += "\"led1\":";
  publishMessage += status_1; // one decimal place
  publishMessage += ",";
  publishMessage += "\"led2\":";
  publishMessage += status_2;
  publishMessage += ",";
  publishMessage += "\"led3\":";
  publishMessage += status_3;
  publishMessage += ",";
  publishMessage += "\"buzzer\":";
  publishMessage += status_4;
  publishMessage += "}"; 
  publishMessage += "}"; 
  
  Serial.println("Publish message: " + publishMessage);
  mqttClient.publish(topic_pub, publishMessage.c_str()); // publish to topic for subscriber
  mqttClient.publish(data_pub, publishMessage.c_str()); // publish to topic for data shadow
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message;
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // MARK: Message Payload
  // message payload: {"led1":1} or {"led1":0}
  if (message.indexOf("\"led1\":1") != -1) {
    digitalWrite(RELAY_LED_RED_PIN, HIGH);
    outputState[0] = 1;
    Serial.println("LED1 is ON");
  } 
  else if (message.indexOf("\"led1\":0") != -1) {
    digitalWrite(RELAY_LED_RED_PIN, LOW);
    outputState[0] = 0;
    Serial.println("LED1 is OFF");
  }
  
  if (message.indexOf("\"led2\":1") != -1) {
    digitalWrite(RELAY_LED_YELLOW_PIN, HIGH);
    outputState[1] = 1;
    Serial.println("LED2 is ON");
  } 
  else if (message.indexOf("\"led2\":0") != -1) {
    digitalWrite(RELAY_LED_YELLOW_PIN, LOW);
    outputState[1] = 0;
    Serial.println("LED2 is OFF");
  }
  
  if (message.indexOf("\"led3\":1") != -1) {
    digitalWrite(RELAY_LED_GREEN_PIN, HIGH);
    outputState[2] = 1;
    Serial.println("LED3 is ON");
  } 
  else if (message.indexOf("\"led3\":0") != -1) {
    digitalWrite(RELAY_LED_GREEN_PIN, LOW);
    outputState[2] = 0;
    Serial.println("LED3 is OFF");
  }
  
  if (message.indexOf("\"buzzer\":1") != -1) {
    digitalWrite(BUZZER_PIN, HIGH);
    outputState[3] = 1;
    Serial.println("Buzzer is ON");
  } 
  else if (message.indexOf("\"buzzer\":0") != -1) {
    digitalWrite(BUZZER_PIN, LOW);
    outputState[3] = 0;
    Serial.println("Buzzer is OFF");
  }
  
  publishLightStatus(String(outputState[0]).c_str(), String(outputState[1]).c_str(), String(outputState[2]).c_str(), String(outputState[3]).c_str());
}

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(mqttCallback);
  pinMode(RELAY_LED_RED_PIN, OUTPUT);
  pinMode(RELAY_LED_YELLOW_PIN, OUTPUT);
  pinMode(RELAY_LED_GREEN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(RELAY_LED_RED_PIN, LOW); // turn the LED off by making the voltage LOW
  digitalWrite(RELAY_LED_YELLOW_PIN, LOW);
  digitalWrite(RELAY_LED_GREEN_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  reconnectMQTT();
  publishLightStatus("0", "0", "0", "0"); // initial status
}

void loop()
{
  if (!mqttClient.connected())
  {
    reconnectMQTT();
  }
  mqttClient.loop();

  delay(1000); // wait for next read
}