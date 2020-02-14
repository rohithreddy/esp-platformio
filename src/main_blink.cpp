#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#define LED_BUILTIN 2

const char ssid[] = "Insert Wifi SSID";
const char pass[] = "Insert Password";
const char mqtt_broker_url[] = "";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;


void blinkled() {
  digitalWrite(LED_BUILTIN, LOW); // turn LED on by making voltage high
  delay(1000);// wait for a second
  digitalWrite(LED_BUILTIN, HIGH);// turn the LED off by making the voltage LOW
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduinoesp", "try", "try")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
  client.subscribe("/hello_again_badc");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  if (payload == "blink") {
    blinkled();
    Serial.println("blink :D :)");
  }
  if (payload == "clog") {
    blinkled();
    Serial.println("console ping --> :) :|");
  }
  Serial.println("blink: " + topic + " - " + payload);
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  client.begin("broker.hivemq.com", net);
  client.onMessage(messageReceived);
  connect();
  pinMode(LED_BUILTIN, OUTPUT);
  blinkled();
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000000) {
    lastMillis = millis();
    client.publish("/hello_again_badc", "rohith");
  }
}
