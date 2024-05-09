#include <WiFi.h>
#include <PubSubClient.h>

// WiFi Credentials
const char* ssid = "ssid";
const char* password = "password";

const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";


#define LED 25    //built-in LED is pin 2

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    
    if (client.connect("userid")) {      //user id must be unique in case of many users under one topic to avoid conflict/crosstalk information
      Serial.println("Connected to MQTT");
      client.subscribe("LED");            //SUBSCRIBE TOPIC
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  
  pinMode(LED, OUTPUT);
}

void loop() 
{}

void callback(String topic, byte* payload, unsigned int length) {
  Serial.println();
  Serial.print("Message received on topic: ");
  Serial.println(topic);
  Serial.println("Message: ");
  String msg;

  if (topic == "LED"){
    for (int i = 0; i < length; i++) {
      //Serial.print((char)payload[i]);
      msg += (char)payload[i];
    }
    Serial.println(msg);
    Serial.print("LED status: ");
    if (msg == "ON"){ 
      digitalWrite(LED, HIGH);
      Serial.println("ON"); 
    }
    else if (msg == "OFF"){ 
      digitalWrite(LED, LOW);
      Serial.println("OFF");
    }
  }
  Serial.println();
  delay(10);   //adjust interval
}

