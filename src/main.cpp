#include <Arduino.h>
#include "wifi-setup.h"
#include "web-server-setup.h"
#include "relay-control.h"

void setup() {
  Serial.begin(115200);
  Serial.println("System is running!");

  pinMode(relayPin, OUTPUT);

  setupWiFi();
  setupServer();

  startTime = millis(); // Initialize the timer
}

void loop() {
  server.handleClient(); // Handle HTTP requests
  relayControl();        // Control the relay based on timing logic
}
