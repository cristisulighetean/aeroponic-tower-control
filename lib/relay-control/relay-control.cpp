#include "relay-control.h"

const int relayPin = 0;
bool relayOn = true;

// Time variables
const int maxDuration = 20 * 60 * 1000;
const int minDuration = 5 * 60 * 1000;
const int period = 60 * 60 * 1000; // 60 minutes

unsigned long startTime = 0;
// Initialize remaining time
long remainingTimeOn = 0; 
long remainingTimeOff = 0;


void relayControl() {
  // Get current time
  unsigned long now = millis();

  // Sync relayOn with actual relay
  if (relayOn) {
    digitalWrite(relayOn, HIGH);
  }
  else {
    digitalWrite(relayOn, LOW);
  }

  // Handle time update in automatic mode
  if (!controlMode) { // Automatic mode logic
    if (relayOn) {
      // Update remaining time ON
      remainingTimeOn = onDuration - (now - startTime);
      // Turn off relay if remaining time is up
      if (remainingTimeOn <= 0) {
        digitalWrite(relayPin, LOW);
        relayOn = false;
        startTime = now; // Reset start time
        remainingTimeOff = offDuration; // Reset remaining time OFF
        Serial.println("AUTO: time out - relay is disabled");
      }
    } else {
      // Update remaining time OFF
      remainingTimeOff = offDuration - (now - startTime);
      // If offDuration is reached, turn on the relay
      if (remainingTimeOff <= 0) {
        digitalWrite(relayPin, HIGH);
        relayOn = true;
        startTime = now;
        remainingTimeOn = onDuration; // Reset remaining time ON
        Serial.println("AUTO: time in - relay is enabled");
      }
    }
  } else { // Manual mode logic
    digitalWrite(relayPin, relayOn ? HIGH : LOW);
  }
}