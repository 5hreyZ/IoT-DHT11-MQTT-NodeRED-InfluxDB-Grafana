# 🌐 IoT-DHT11-MQTT-NodeRED-InfluxDB-Grafana

### 📋 Description
End-to-end IoT data pipeline using NodeMCU (ESP8266) and DHT11 to measure temperature and humidity.  
Data is published via MQTT to a Raspberry Pi running Mosquitto, processed with Node-RED, stored in InfluxDB,  
and visualized in Grafana dashboards for real-time monitoring and analysis.

---

### 🎯 Objective
- Implement an IoT system where a NodeMCU (ESP8266) equipped with a DHT11 sensor publishes temperature and humidity data to a Mosquitto MQTT broker running on a Raspberry Pi.  
- Process and subscribe to MQTT data using Node-RED.  
- Store processed data in InfluxDB.  
- Visualize real-time temperature and humidity data using Grafana dashboards.

---

### 🧩 System Overview
#### 📊 Data Flow:
