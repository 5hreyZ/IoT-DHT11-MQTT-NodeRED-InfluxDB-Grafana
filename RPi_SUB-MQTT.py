#!/usr/bin/env python3
import csv
import json
from datetime import datetime
import paho.mqtt.client as mqtt
import os

# ------------------- CONFIG -------------------
BROKER = "localhost"        # or your broker IP if running elsewhere
PORT = 1883
TOPIC = "Shrey/Lab4"        # topic from ESP8266
CSVFILE = "dht_log.csv"
# ------------------------------------------------

# Create CSV file with header if not existing
if not os.path.exists(CSVFILE):
    with open(CSVFILE, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow([
            "timestamp_utc_iso",
            "topic",
            "temperature_C",
            "humidity_percent",
            "payload_raw"
        ])

# ------------------- CALLBACKS -------------------
def on_connect(client, userdata, flags, rc):
    print(f"Connected to broker (rc={rc}). Subscribing to topic: {TOPIC}")
    client.subscribe(TOPIC)

def on_message(client, userdata, msg):
    ts = datetime.utcnow().isoformat() + "Z"
    payload = msg.payload.decode(errors="replace")
    print(f"[{ts}] {msg.topic} -> {payload}")

    temp = hum = None
    try:
        pdata = json.loads(payload)
        temp = pdata.get("temperature")
        hum = pdata.get("humidity")
    except Exception as e:
        print(f"Error parsing JSON: {e}")
        pdata = None

    # Append to CSV
    with open(CSVFILE, "a", newline="") as f:
        writer = csv.writer(f)
        writer.writerow([ts, msg.topic, temp, hum, payload])

# ------------------- MAIN -------------------
if __name__ == "__main__":
    client = mqtt.Client(client_id="", protocol=mqtt.MQTTv311)
    client.on_connect = on_connect
    client.on_message = on_message

    try:
        print(f"Connecting to MQTT broker at {BROKER}:{PORT}...")
        client.connect(BROKER, PORT, 60)
        client.loop_forever()
    except KeyboardInterrupt:
        print("\nExiting...")
    except Exception as e:
        print(f"Connection error: {e}")
