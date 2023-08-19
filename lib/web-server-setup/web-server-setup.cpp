#include "web-server-setup.h"
#include "relay-control.h"

ESP8266WebServer server(80);
unsigned long int onDuration = 15 * 60 * 1000; // Default 15 minutes
unsigned long int offDuration = 45 * 60 * 1000; // Corresponding off duration
bool manualMode = false; // Start in automatic mode by default

void updateOffDuration() {
  offDuration = period - onDuration;
}

void setupServer() {
  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.on("/status", handleStatus);
  server.on("/manual", handleManual);
  server.on("/toggle", handleToggle);
  server.begin();
}

void handleRoot() {
  String html = "<html>";
  html += "<head>";
  html += "<style>";
  html += "  body { background-color: #8ea604; font-family: Arial, sans-serif; }"; // CSS for background and font
  html += "  .container { display: flex; flex-direction: column; align-items: center; justify-content: center; min-height: 80vh; }"; // Center-align everything inside the container
  html += "  form { text-align: center; }"; // Center-align text inside the form
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<div class='container'>";
  html += "<h1>Aeroponic Tower Control</h1>";
  html += "<p>Current Mode: " + String(manualMode ? "Manual" : "Automatic") + "</p>";
  html += "<p>Relay Status: " + String(relayOn ? "ON" : "OFF") + "</p>";

  if (!manualMode) {
    if (relayOn) {
      html += "<p>Remaining Time ON: " + String(remainingTimeOn / 1000 / 60) + " minutes</p>";
    } else {
      html += "<p>Remaining Time OFF: " + String(remainingTimeOff / 1000 / 60) + " minutes</p>";
    }
  }

  if (manualMode) {
    html += "<p><a href='/toggle'>Toggle Relay</a></p>";
  }
  html += "<p><a href='/manual'>Switch to " + String(manualMode ? "Automatic" : "Manual") + " Mode</a></p>";
  html += "<form action='/set' method='get'>";
  html += "  <label for='duration'>Set ON Duration (minutes, from a 60-minute period): </label>";
  html += "  <input type='number' id='duration' name='duration' min='5' max='20'>"; // Min 5, Max 20 minutes
  html += "  <input type='submit' value='Set'>";
  html += "</form>";
  html += "</div>";
  html += "</body>";
  html += "</html>";
  server.send(200, "text/html", html);
}


void handleSet() {
  int newDuration = server.arg("duration").toInt() * 60 * 1000;
  if (newDuration >= minDuration && newDuration <= maxDuration) {
    onDuration = newDuration;
    updateOffDuration();
    server.send(200, "text/plain", "Duration set successfully");
  } else {
    server.send(400, "text/plain", "Value out of bounds");
  }
}

// Show status in plain text
void handleStatus() {
  String status = "Mode: " + String(manualMode ? "Manual" : "Automatic");
  status += "\nRelay Status: " + String(relayOn ? "ON" : "OFF");
  status += "\nOn Duration (Automatic Mode): " + String(onDuration / 60 / 1000) + " minutes";

  if (!manualMode) {
    if (relayOn) {
        status += "<p>Remaining Time On: " + String(remainingTimeOn / 1000 / 60) + " minutes</p>";
    }
    else {
        status += "<p>Remaining Time OFF: " + String(remainingTimeOff / 1000 / 60) + " minutes</p>";
    }
  }

  server.send(200, "text/plain", status);
}

void handleManual() {
  manualMode = !manualMode; // Toggle mode
  if (!manualMode) { // If switched to automatic mode
    startTime = millis();   // Reset the relay control timer
    relayOn = !relayOn;     // Turn on relay if switched to automatic mode
  }
  server.sendHeader("Location", "/");
  server.send(303); // Redirect to root
}

void handleToggle() {
  if (manualMode) { // Only allow toggling in manual mode
    relayOn = !relayOn;
    digitalWrite(relayPin, relayOn ? HIGH : LOW);
  }
  server.sendHeader("Location", "/");
  server.send(303); // Redirect to root
}
