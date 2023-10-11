#include "HTMLData.h"
#include "WifiConfig.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>



ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

ScooterController scooter;



const byte* intToBytes(unsigned int speedKmph) {
    static byte newData[2];
    newData[0] = speedKmph & 0xFF;     // Low byte
    newData[1] = speedKmph >> 8;       // High byte
    return newData;
}

String byteArrayToString(byte* buffer, int length) {
  String result = "";
  for (int i = 0; i < length; i++) {
    if (buffer[i] < 0x10) {
      result += "0";
    }
    result += String(buffer[i], HEX);
    result += " ";
  }
  return result;
}

void handleRoot() {
  httpServer.send(200, "text/html", HTML_DATA);
}

void handleSetSpeed() {
  String mode = httpServer.arg("mode"); // race, sport, eco
  String speed = httpServer.arg("speed");
  int speedValue = speed.toInt();
  
  byte* packet = nullptr;
  int packetSize = 0;

  if (mode == "race") {
    packet = scooter.sendCommand(ScooterController::RACE_SPEED_LIMIT, intToBytes(speedValue));
  } else if (mode == "sport") {
    packet = scooter.sendCommand(ScooterController::SPORT_SPEED_LIMIT, intToBytes(speedValue));
  } else if (mode == "eco") {
    packet = scooter.sendCommand(ScooterController::ECO_SPEED_LIMIT, intToBytes(speedValue));
  }

  if (packet) {
    packetSize = scooter.lastSentPacketSize;
    String packetStr = byteArrayToString(packet, packetSize);
    httpServer.send(200, "text/plain", "Mode: " + mode + " Speed set to: " + speed + " Packet: " + packetStr);
  } else {
    httpServer.send(200, "text/plain", "Mode: " + mode + " Speed set to: " + speed + " Packet: error");
  }
}

void handleSetSpeedLimit() {
  String speedLimitStr = httpServer.arg("speedLimit");
  int limit = speedLimitStr.toInt();
  
  if(limit >= 0 && limit <= 255) {
    scooter.saveToEEPROM("speedLimit", limit);
    httpServer.send(200, "text/plain", "Speed Limit set to: " + speedLimitStr);
  } else {
    httpServer.send(400, "text/plain", "Invalid Speed Limit");
  }
}





void startWiFi() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(AP_SSID, AP_PSK);
  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) { // 10 second timeout
    delay(100);
    Serial.println("Trying to connect...");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi, but AP is active!");
  } else {
    Serial.println("Connected to WiFi and AP is active!");
  }

}

void startHttp() {
  
  MDNS.begin(host);

  httpUpdater.setup(&httpServer);
  httpServer.on("/", handleRoot);
  httpServer.on("/set_speed", handleSetSpeed);
  httpServer.on("/set_speed_limit", handleSetSpeedLimit);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
}
