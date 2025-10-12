#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// -------------------- USER CONFIG --------------------
#define DHTPIN D4          // GPIO2 on NodeMCU (adjust if using another pin)
#define DHTTYPE DHT11      // or DHT22 depending on your sensor

const char* WIFI_SSID = "5hreyZ";
const char* WIFI_PASSWORD = "12345678s";

const char* MQTT_BROKER_IP = "10.96.38.201"; 
const uint16_t MQTT_PORT = 1883;
const char* MQTT_TOPIC = "Shrey/Lab4"; // Topic where data is published

// -----------------------------------------------------

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastPublish = 0;
const long publishInterval = 5000; // 5 seconds

// -------------------- WIFI --------------------
void connectWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// -------------------- MQTT --------------------
void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("esp8266-dht")) {
      Serial.println("connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retry in 2s");
      delay(2000);
    }
  }
}

// -------------------- SETUP --------------------
void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();
  connectWiFi();
  client.setServer(MQTT_BROKER_IP, MQTT_PORT);
}

// -------------------- LOOP --------------------
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastPublish > publishInterval) {
    lastPublish = now;

    float h = dht.readHumidity();
    float t = dht.readTemperature(); // Celsius by default

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Prepare payload
    char payload[150];
    snprintf(payload, sizeof(payload),
             "{\"temperature\": %.2f, \"humidity\": %.2f, \"timestamp_ms\": %lu}",
             t, h, now);

    // Print to Serial Monitor
    Serial.print("Publishing: ");
    Serial.println(payload);

    // Publish to MQTT
    if (client.publish(MQTT_TOPIC, payload)) {
      Serial.println("MQTT publish successful ✅");
    } else {
      Serial.println("MQTT publish failed ❌");
    }
  }
}
