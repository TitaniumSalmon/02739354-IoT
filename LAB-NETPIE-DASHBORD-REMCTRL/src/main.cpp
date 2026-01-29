#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
// WIFI config
const char *ssid = "Wokwi-GUEST";
const char *password = "";
WiFiClient wifiClient;
// MQTT client config
const char *mqttServer = "mqtt.netpie.io";
const int mqttPort = 1883;
const char *mqttClientId = "d4eb1908-baf9-462f-9548-65c677d84b1e";
const char *mqttUser = "w7uMyBTGKrUA5hUdTRiehvNGk8Ykk6j9";
const char *mqttPassword = "vN3fFicUw6HaE1v168HkxvsLiSy1yLC9";
const char *topic_sub = "@msg/home/device_control"; // topic to subscribe
const char *topic_pub = "@msg/home/device_status"; // topic to publish
const char *data_pub = "@shadow/data/update"; // topic to publish
PubSubClient mqttClient(wifiClient);

// pin definition
const int RELAY_PIN = 12; 

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

void publishLightStatus(const char* status) {
  String publishMessage;
  // Prepare JSON message
  publishMessage = "{\"data\":";
  publishMessage += "{\"led1\":";
  publishMessage += status; // one decimal place
  publishMessage += "}";
  publishMessage += "}"; 
  
  Serial.println("Publish message: " + publishMessage);
  mqttClient.publish(topic_pub, publishMessage.c_str()); // publish to topic for subscriber
  mqttClient.publish(data_pub, publishMessage.c_str()); // publish to topic for data shadow
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message, lightStatus, publishMessage;
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
  
  // message payload: {"led1":1} or {"led1":0}
  if (message.indexOf("\"led1\":1") != -1) {
    digitalWrite(RELAY_PIN, HIGH); // turn the LED on (HIGH is the voltage level)
    lightStatus = "1";
    Serial.println("LED is ON");
  } 
  else if (message.indexOf("\"led1\":0") != -1) {
    digitalWrite(RELAY_PIN, LOW); // turn the LED off by making the voltage LOW
    lightStatus = "0";
    Serial.println("LED is OFF");
  }
  // send the status to the topic_pub
  publishLightStatus(lightStatus.c_str());
}

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(mqttCallback);
  pinMode(RELAY_PIN, OUTPUT);  
  digitalWrite(RELAY_PIN, LOW); // turn the LED off by making the voltage HIGH
  reconnectMQTT();
  publishLightStatus("0"); // initial status
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