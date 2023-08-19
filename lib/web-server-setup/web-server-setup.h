#ifndef WEB_SERVER_SETUP_H
#define WEB_SERVER_SETUP_H

#include <ESP8266WebServer.h>

extern unsigned long int onDuration;
extern unsigned long int offDuration;
extern ESP8266WebServer server;
extern bool manualMode;

void setupServer();
void handleRoot();
void handleSet();
void handleStatus();
void handleManual(); 
void handleToggle();

#endif
