#include "wifi-setup.h"
#include <ESP8266WiFi.h>

const char* ssid = "Aeroponic Tower Control";
const char* password = "hyperveggies";

void setupWiFi() {
  // Create Access Point
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  
  Serial.println("Starting WiFi Access Point");
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}
