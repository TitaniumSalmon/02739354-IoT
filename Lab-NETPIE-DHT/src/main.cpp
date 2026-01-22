#include <Arduino.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <WiFi.h>
// WIFI config
const char *ssid = "Wokwi-GUEST";
const char *password = "";
WiFiClient wifiClient;
// MQTT client config
const char *mqttServer = "mqtt.netpie.io";
const int mqttPort = 1883;
const char *mqttClientId = "52ef9968-6776-4eca-a173-3e49dc7e5985"; // replace with your client ID
const char *mqttUser = "r97ooQU3Lo1NZPMAPcnYorYmWsMyR3tz"; // replace with your MQTT username
const char *mqttPassword = "9mSutYsVyV73GMj2vzzX9ytRWWM7zfHe"; // replace with your MQTT password
const char *topic_pub = "@msg/lab_ict_kps/sensor_data"; // topic to publish
const char *data_pub = "@shadow/data/update";
PubSubClient mqttClient(wifiClient);
// some variables buffer
String publishMessage;
// pin definition
const int DHT_PIN = 13;
const int LED_PIN = 14; 

DHT dht(DHT_PIN, DHT22);

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
      // à¹ƒà¸™à¸•à¸±à¸§à¸­à¸¢à¹ˆà¸²à¸‡à¸‚à¸­à¸‡à¸à¸±à¹ˆà¸‡à¸ªà¹ˆà¸‡à¹„à¸¡à¹ˆà¸¡à¸µà¸à¸²à¸£ subscribe topic à¹ƒà¸”
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

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  mqttClient.setServer(mqttServer, mqttPort);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);  
  delay(1000); // delay to let initialization ready  
}

void loop()
{
  digitalWrite(LED_PIN, LOW); // turn the LED off by making the voltage LOW
  if (!mqttClient.connected())
  {
    reconnectMQTT();
  }
  mqttClient.loop();
  delay(1000); // total waiting 2 seconds for next read
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  digitalWrite(LED_PIN, HIGH); // turn the LED on (HIGH is the voltage level)
// Prepare JSON message
  publishMessage = "{\"data\":";
  publishMessage += "{\"temperature\":";
  publishMessage += String(temperature, 1); // one decimal place
  publishMessage += ",\"humidity\":";
  publishMessage += String(humidity, 1);    // one decimal place
  publishMessage += "}";
  publishMessage += "}"; 
  
  Serial.println("Publish message: " + publishMessage);
  mqttClient.publish(topic_pub, publishMessage.c_str());
  mqttClient.publish(data_pub, publishMessage.c_str());
 
  delay(1000); // wait for next read
}