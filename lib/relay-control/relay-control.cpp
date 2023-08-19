#include "relay-control.h"

const int relayPin = 0;
unsigned long startTime = 0;
bool relayOn = true;
const int maxDuration = 20 * 60 * 1000;
const int minDuration = 5 * 60 * 1000;
const int period = 60 * 60 * 1000; // 60 minutes

unsigned long remainingTimeOn = 0; // Initialize remaining time for ON
unsigned long remainingTimeOff = 0; // Initialize remaining time for OFF


void relayControl() {
  unsigned long now = millis();

  if (!manualMode) { // Automatic mode logic
    if (relayOn) {
      // Update remaining time ON
      remainingTimeOn = onDuration - (now - startTime);
      // Turn off relay if remaining time is up
      if (remainingTimeOn <= 0) {
        digitalWrite(relayPin, HIGH); // Relay logic is inverted
        relayOn = false;
        startTime = now; // Reset start time
        remainingTimeOff = offDuration; // Reset remaining time OFF
        Serial.println("Time out: Relay is disabled");
      }
    } else {
      // Update remaining time OFF
      remainingTimeOff = offDuration - (now - startTime);
      // If offDuration is reached, turn on the relay
      if (remainingTimeOff <= 0) {
        digitalWrite(relayPin, LOW); // Relay logic is inverted
        relayOn = true;
        startTime = now;
        remainingTimeOn = onDuration; // Reset remaining time ON
        Serial.println("Time in: Relay is enabled");
      }
    }
  } else { // Manual mode logic
    digitalWrite(relayPin, relayOn ? LOW : HIGH);
  }
}
