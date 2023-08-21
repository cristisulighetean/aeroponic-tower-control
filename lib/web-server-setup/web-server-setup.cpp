#include "web-server-setup.h"
#include "relay-control.h"

ESP8266WebServer server(80);
unsigned long int onDuration = 15 * 60 * 1000; // Default 15 minutes
unsigned long int offDuration = 45 * 60 * 1000; // Corresponding off duration
bool controlMode = false; // False = automatic, true = manual mode

void updateOffDuration() {
  offDuration = period - onDuration;
}

void setupServer() {
  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.on("/modeswitch", handleModeSwitch);
  server.on("/toggle", handleManualToggle);
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
  html += "<p>Current Mode: " + String(controlMode ? "Manual" : "Automatic") + "</p>";
  html += "<p>Relay Status: " + String(relayOn ? "ON" : "OFF") + "</p>";

  // Show only in automatic mode
  if (!controlMode) {
    if (relayOn) {
      html += "<p>Remaining Time ON: " + String(remainingTimeOn / 1000 / 60) + " minutes</p>";
    } else {
      html += "<p>Remaining Time OFF: " + String(remainingTimeOff / 1000 / 60) + " minutes</p>";
    }
  }

  // Show only in manual mode
  if (controlMode) {
    html += "<p><a href='/toggle'>Toggle Relay</a></p>";
  }

  html += "<p><a href='/modeswitch'>Switch to " + String(controlMode ? "Automatic" : "Manual") + " Mode</a></p>";
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
  String message;
  if (newDuration >= minDuration && newDuration <= maxDuration) {
    onDuration = newDuration;
    updateOffDuration();
    message = "Duration set successfully";
  } else {
    message = "Value out of bounds";
  }

  String html = "<html><head><script>";
  html += "alert('" + message + "');";
  html += "window.location.href = '/';"; // Redirect back to the root
  html += "</script></head><body></body></html>";
  server.send(200, "text/html", html);
}

void handleModeSwitch() {
  controlMode = !controlMode; // Toggle mode
  
  // Switch to manual mode
  if (controlMode) {
    relayOn = false;  // Turn off relay
  }
  else { // Switch to automatic mode
    startTime = millis();   // Reset the relay control timer
    relayOn = true;     // Turn on relay
  }

  // Redirect to root
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleManualToggle() {
  if (controlMode) { // Only allow toggling in manual mode
    relayOn = !relayOn;
  }

  // Redirect to root
  server.sendHeader("Location", "/");
  server.send(303);
}
