#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <MQTTClient.h>


const char* host = "192.168.178.157";
const char* ssid = "*******";
const char* password = "************";

const int relayPin = D1;

WiFiClient net;
MQTTClient mqtt;

void connect();

void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting...");

  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  mqtt.begin(host, net);
  mqtt.onMessage(messageReceived);

  connect();
  
  pinMode(relayPin, OUTPUT);
  
  Serial.println("Setup completed...");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!mqtt.connected()) {
    connect();
  }

  mqtt.loop();
  delay(3000);

  void onMessage(MQTTClientCallbackSimple);
}

void connect() {
  while(WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("WiFi connection failed. Retry.");
  }

  Serial.print("Wifi connection successful - IP-Address: ");
  Serial.println(WiFi.localIP());

  while (!mqtt.connect(host)) {
    Serial.print(".");
  }

  Serial.println("MQTT connected!");

  mqtt.subscribe("/pumpe/cmd/");

  mqtt.publish("/pumpe/status/", "bin da!");
  
}

void messageReceived(String &topic, String &payload) {
  Serial.print("Message received! ....");
  Serial.print(topic);
  Serial.print("....Payload: ");
  Serial.print(payload);
  if (payload == "true") {
    digitalWrite(relayPin, HIGH);
    mqtt.publish("/pumpe/status/", "true");
  };
  if (payload == "false") {
    digitalWrite(relayPin, LOW);
    mqtt.publish("/pumpe/status/", "false");
  };
  
  }


